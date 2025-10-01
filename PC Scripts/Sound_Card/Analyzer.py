import soundcard as sc
import numpy as np
import threading
import queue
import time
import serial
import matplotlib
matplotlib.use("TkAgg")
import matplotlib.pyplot as plt

def main(comm,ard=False,plot=False,verbose=False):
	
	samplerate  = 48000                # 48kHz is standard rate for audio, mp3 uses it for example
	blocksize   = 512                  # Number of samples per block, tradeoff between delay and frequency resolution
	scaling     = 3                    # Magic number to get nicer volume numbers, I find 3 to be quite good
	sample_time = blocksize/samplerate # Time in seconds of each sample block

	# Pick the "loopback microphone" corresponding to the output device
	# You can list all devices with sc.all_microphones(include_loopback=True)
	# Replace the string with the exact name from the list
	#loopback_mic = sc.get_microphone('Bose NC 700 HP', include_loopback=True)

	# In this case Bose NC 700 HP has a physical mic as well as a loopback mic
	# so for now I just hard coded the second device from the list (the loopback one)
	loopback_mic = sc.all_microphones(include_loopback=True)[1]

	if verbose:
		# Nerd stats
		print("\n Nerd stats:")
		print("  Using device:", loopback_mic)
		print(f"  Sample rate : {samplerate/1000:.3f} kHz")
		print(f"  Block size  : {blocksize} samples")
		print(f"  Sample time : {sample_time*1000:.3f} ms")
		print("  Scaling     :", scaling)
		print("")

	# Set up threading event to stop the analysis thread gracefully
	stop_event = threading.Event()

	# Create a queue to communicate between threads
	q = queue.Queue()

	# Live plotting setup
	if plot:
		plt.ion()
		fig, ax = plt.subplots(figsize=(3,6))
		ax.set_ylim(0, 1)  # since volume_RMS is clamped [0,1]
		ax.set_xlim(-0.5, 0.5)
		ax.set_xticks([])
		ax.set_ylabel("Volume (RMS)")
		ax.set_title("Live Audio Volume")

		# Single vertical bar
		bar = ax.bar(0, 0, width=0.5, color="red")

	# This will be run in a separate thread
	def audio_analysis():
		""" Accessses loopback mic and computes RMS volume for every blocksize samples """

		with loopback_mic.recorder(samplerate=samplerate, blocksize=blocksize) as rec:
	
			print(" Accessing loopback recording...")
			
			# Loop until stop_event is set (on KeyboardInterrupt)
			while not stop_event.is_set():
				# The actual audio data
				data = rec.record(numframes=blocksize)

				# --Here's where you can do some analysis!--

				# Compute RMS volume
				volume_RMS = np.sqrt(np.mean(data**2))*scaling # Magic number scaling to get nicer numbers
				
				if volume_RMS > 1.0:
					# Clamp to [0, 1], important later since we can only send 1 byte to the arduino
					volume_RMS = 1.0 

				if ard:
					# Scale to [0, 255] for sending as byte
					to_send = int(volume_RMS * 255) 
					volume_RMS = to_send
					
					# Send over serial
					comm.write(int.to_bytes(to_send, 1, "little")) # Arduino uses little endian
				elif plot:
					try:
						# Push volume to queue for plotting
						q.put_nowait(volume_RMS)
					except queue.Full:
						pass

				print(f" Volume: {volume_RMS:.4f}", end="\r")

			print("##### Analysis thread killed #####\n")

	# Start thread, the daemon=True means it will automatically close when main thread closes
	analysis_thread = threading.Thread(target=audio_analysis, daemon=True)
	analysis_thread.start()

	try:
		while True:
			time.sleep(0.025)  # ~40 FPS plotting
			
			# Note that this also handles the non-plotting case, since then q will always be empty
			if not q.empty():
				
				# Drain queue and only use the latest value
				while not q.empty():
					value = q.get_nowait()
				
				# Update bar height
				bar[0].set_height(value)
				fig.canvas.draw_idle()
				fig.canvas.flush_events()
	except KeyboardInterrupt:
		
		# Kill analysis thread if Ctrl+C is pressed
		print("")
		print(" (!) Killing analysis thread")
		stop_event.set()
		analysis_thread.join()

		# Close serial connection
		if ard:
			comm.close()

if __name__ == "__main__":

	verbose = True
	ard     = True
	plot    = False

	print("\n##### Starting Analysis #####")
	print(" Press Ctrl+C to stop")

	if ard:
		comm = serial.Serial(port='COM4',baudrate=115200,timeout=0.1) # with this baud rate a byte is sent every ~0.00008680555 seconds
		time.sleep(1) #give the connection a second to settle
		comm.write(10)
	else:
		comm = None

	main(comm,ard=ard,plot=plot,verbose=verbose)
	