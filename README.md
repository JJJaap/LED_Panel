# LED_Panel
Repo for documentation and software related to getting the homemade LED panel up and running. The panel is supposed to respond to music in various ways, such as in reaction to volume or perhaps some spectral analysis.

# Hardware

The LED Panel currently requires 5 components:

1. A strip of individually controllable LEDs (WS2812b)
2. A switching power supply (LRS-50-5) which provides up to 10A at 5V
3. An Arduino Uno
4. An external computer
5. A breadboard

An individual WS2812b pixel consists of 3 LEDs; red, green and blue. Taken together the whole pixel draws at most 60 mA at full brightness. Considering a 17 by 17 pixel LED panel, the whole thing would require 17.34A at full brightness. Since we are limited by the LRS-50-5 power supply to 10A, we have to limit the max brightness to about 0.58 of the max brightness (in practice it is limited to 0.5).

The arduino handles the controlling of all the LEDs using a library called FastLED. The audio streaming and processing is done with the external computer. It can communicate with the arduino in order to tell the LEDs how to respond. Currently it sends the RMS volume scaled to fit into [0,255] as a byte which the arduino uses to light up a certain amount of LEDs.

The breadboard is there to connect the LEDS, power supply and arduino. See the wiring scheme below.

I still need to implement a power injection scheme, there needs to be an additional direct connection to the power supply +V and GND about every 50 LEDs to avoid voltage drop-off.

## Wiring Scheme

Currently a breadboard is employed to connect the power supply, arduino and LEDs. There is a 1uF capacitor across the power supply to smooth it out (since it is a switching power supply) as well as to pick up sudden changes in current (a lot of LEDs turn on for example). There is a 330 Ohm resistor between the Arduino and the LEDS to protect the Arduino from surges. Note that the Arduino ground is connected to the negative terminal of the power supply. This will not be sufficient for anything above 1A really, so a better solution should be found.

# Software

As mentioned the Arduino handles the controlling of the LEDs. This is done primarily through a library called FastLEDS, which smoothly and neatly handles all communication. It has a wide array of functions which can be used to control the LEDs as required.

On the computer side python is used to record and analyze audio streams, as well as sending any relevant information over serial communication to the arduino. A python library called SoundCard is used to access the built in virtual loop-back recording microphone connected to the relevant speaker or audio device. This recording returns a time-ordered array of amplitudes which are usually sent to a speaker, ranging from -1 to 1, which can then be analyzed. As an example we can take, say, a 5ms recording. Then, the root mean square volume could be calculated, scaled to be between 0 and 255, and then sent as a byte to the arduino over the serial bus. If the recordings are taken continously in quick succession, we get a fairly continuous changing volume being sent to the arduino (fast enough that humans don't notice).

## Spectral Analysis

This is not yet implemented, but some problems can already be ascertained. For one, the discrete Fourier transforms we will be performing require a sample of a certain length. Specifically, the frequency resolution is equal to the sampling frequency divided by the number of samples. We are using the standard 48 kHz sampling frequency. If we take a sample size of 512, this would mean we have a resolution of 93.75 Hz. For reference, the difference between the lowest C on the piano (C1) and its sharp is about 2Hz. To obtain this resolution with the current sampling frequency we would require a sample size of 22,000 samples, equating to a 0.5 second sample. Without even considering any delay due to computation and communications, this 0.5 second delay would be very noticeable; the visuals would lag half a second behind the music.

In terms of human perception, the limit of delay that we can notice is about 10-20 ms, so this should be the goal.

