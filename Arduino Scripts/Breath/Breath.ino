#include <FastLED.h> 
#define NUM_LEDS 100
#define DATA_PIN 2 //How boring and obvious!
#define COLOR_ORDER GRB //Green (G), Red (R), Blue (B)
#define CHIPSET WS2812B
#define BRIGHTNESS 100 
#define VOLTS 5 
#define MAX_AMPS 5000 //value in milliamps

//ENOUGH NONSENSE!

CRGB leds[NUM_LEDS];
int DEL=0;

void setup() {

FastLED.addLeds<CHIPSET,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS);
FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_AMPS);
FastLED.setBrightness(BRIGHTNESS);
FastLED.clear();
FastLED.show(); 

//LEDIA FIVE IS ALIVE!!!!!!!!!!!!!!!!!!!!

}

void loop() { //Breathing effect!

for (int i=0; i<NUM_LEDS; i++) {
  leds[i] = CRGB(255,0,0); 
}
for (int i=0; i<BRIGHTNESS; i++) {
  FastLED.setBrightness(i);
  FastLED.show();
  delay(DEL);
}
for (int i=BRIGHTNESS; i>=0; i--) {
  FastLED.setBrightness(i);
  FastLED.show();
  delay(DEL);
}

//decrements down

for (int i=NUM_LEDS - 1; i>=0; i--) { 
  leds[i] = CRGB(0,0,255);
}
for (int i=0; i<BRIGHTNESS; i++) {
  FastLED.setBrightness(i);
  FastLED.show();
  delay(DEL);
}
for (int i=BRIGHTNESS; i>=0; i--) {
  FastLED.setBrightness(i);
  FastLED.show();
  delay(DEL);
}

}
