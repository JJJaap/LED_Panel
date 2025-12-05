#include <FastLED.h>
#define NUM_LEDS 187
#define DATA_PIN 2 //How boring and obvious!
#define COLOR_ORDER GRB //Green (G), Red (R), Blue (B)
#define CHIPSET WS2812B
#define BRIGHTNESS 50
#define VOLTS 5
#define MAX_AMPS 10000 //value in milliamps

//ENOUGH NONSENSE!

CRGB leds[NUM_LEDS];
int incomingByte; // for incoming serial data
int thresh;       // scaled incoming byte

void setup() {

FastLED.addLeds<CHIPSET,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS);
FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_AMPS);
FastLED.setBrightness(BRIGHTNESS);
FastLED.clear();
FastLED.show(); 
Serial.begin(115200); // opens serial port, sets data rate to 115200 bps

}

void loop() { //Swirly, twirly effect
 
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    // Write it back for debugging purposes
    Serial.write(incomingByte);
    // The byte scales from 0-255, we need this to scale with our number of available LEDS
    // Thus:
    thresh = map(incomingByte, 0, 255, 0, NUM_LEDS);
    if (thresh > NUM_LEDS) thresh = NUM_LEDS;

  }
  
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < thresh) {
      leds[i] = CHSV(beatsin8(10), 255, 125);
      //leds[i] = CRGB(255,0,0);
    } else {
      
      leds[i].fadeToBlackBy(10);
      
      // If not blue fade toward blue first
      if ((leds[i].r > 0)||(leds[i].g > 0)) {
        leds[i].r = max(0, leds[i].r - 10);   // decrease red
        leds[i].g = max(0, leds[i].g - 10);   // decrease green
        leds[i].b = min(255, leds[i].b + 10); // increase blue
      } else {
        leds[i].fadeToBlackBy(10); // then fade blue to black
      }
    }
  }
  
  // Update the LEDs
  FastLED.show();
}
