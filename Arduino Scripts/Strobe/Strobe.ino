#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 2 //How boring and obvious!
#define COLOR_ORDER GRB //Green (G), Red (R), Blue (B)
#define CHIPSET WS2812B
#define BRIGHTNESS 50
#define VOLTS 5
#define MAX_AMPS 500 //value in milliamps

//ENOUGH NONSENSE!

CRGB leds[NUM_LEDS];
int incomingByte; // for incoming serial data
int DEL=10;


void setup() {

FastLED.addLeds<CHIPSET,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS);
FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_AMPS);
FastLED.setBrightness(BRIGHTNESS);
FastLED.clear();
FastLED.show(); 
Serial.begin(115200); // opens serial port, sets data rate to 115200 bps

//LEDIA IS ALIVE!!!!!!!!!!!!!!!!!!!!

}

void loop() {
  for (int i=0; i<NUM_LEDS; i++) {
    leds[i]=CHSV(15,255,255);
  }
  delay(DEL);
  FastLED.show();
  for (int i=0; i<NUM_LEDS; i++) {
    leds[i]=CHSV(0,255,255);
  }
  delay(DEL);
  FastLED.show();
   for (int i=0; i<NUM_LEDS; i++) {
    leds[i]=CHSV(0,0,255);
  }
  delay(DEL);
  FastLED.show();

}
