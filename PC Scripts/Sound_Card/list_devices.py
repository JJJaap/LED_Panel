import soundcard as sc

inputs = sc.all_microphones(include_loopback=True)

print("\nMicrophones (including loopback):")
for input in inputs:
    print(input)

inputs = sc.all_speakers()

print("\nSpeakers:")
for input in inputs:
    print(input)
print("")