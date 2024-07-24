#include <FastLED.h>
#define NUM_LEDS 6
#define DATA_PIN 2 //How boring and obvious!
#define COLOR_ORDER GRB //Green (G), Red (R), Blue (B)
#define CHIPSET WS2812B
#define BRIGHTNESS 50
#define VOLTS 5
#define MAX_AMPS 500 //value in milliamps

//ENOUGH NONSENSE!

CRGB leds[NUM_LEDS];
int incomingByte; // for incoming serial data


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
    if (i < incomingByte/3) { // I scale the incoming byte here to match the fact that I can only power 6 leds with the arduino
      leds[i] = CHSV(beatsin8(10), 255, 125);
    }
    else {
      leds[i] = CRGB(0, 0, 0);
    }
  }
  FastLED.show();
}
