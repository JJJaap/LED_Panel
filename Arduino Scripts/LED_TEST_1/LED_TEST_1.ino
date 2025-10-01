#include <FastLED.h>
#define NUM_LEDS 187
#define DATA_PIN 2 //How boring and obvious!
#define COLOR_ORDER GRB //Green (G), Red (R), Blue (B)
#define CHIPSET WS2812B
#define BRIGHTNESS 75
#define VOLTS 5
#define MAX_AMPS 500 //value in milliamps

//ENOUGH NONSENSE!

CRGB leds[NUM_LEDS];
int incomingByte; // for incoming serial data
int comp; // scaling thing


void setup() {

  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps
  
  //JONNY FIVE IS ALIVE!!!!!!!!!!!!!!!!!!!!

}

void loop() { //Swirly, twirly effect

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    Serial.write(incomingByte);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    comp = incomingByte/255*NUM_LEDS; // Scale the incoming byte to the number of LEDs
    if (i < incomingByte) { 
      leds[i] = CHSV(beatsin8(20), 255, 125);
    }
    else {
      leds[i] = CRGB(0, 0, 0);
    }
  }
  FastLED.show();
}
