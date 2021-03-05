/*
 * Neopixel
 */


#include <Adafruit_NeoPixel.h>

// Neopixel Config
#define NeoPIN D4 // Neopixel data pin
#define NUM_LEDS 6 // number of leds
int brightness = 150;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NeoPIN, NEO_RGB + NEO_KHZ800);

void startNeoPixel(){
    strip.setBrightness(brightness);
    strip.begin();
    strip.show();
  }


void setNeoColor(int r, int g, int b){
  Serial.print("Setting Neopixel...");
  // DEBUG
  Serial.print("RGB: ");
  Serial.print(r, DEC);
  Serial.print(" ");
  Serial.print(g, DEC);
  Serial.print(" ");
  Serial.print(b, DEC);
  Serial.println(" ");
  
  // setting whole strip to the given color
  for(int i=0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color( g, r, b ) );
  }
  // init
  strip.show();
  
  Serial.println("on.");
}
