#include "SPI.h"
#include "WS2801.h"

// Visual Settings...
int color1[] = {40, 50, 120};
int color2[] = {255, 255, 255};
float bpm = 70; // beats per minute.
int surgeSpeed = 4; // surge growth (in pulses).
float steps = 20; // steps between colors (bg).
int randomness = 0; // color steps to jump around while fading (bg).

// System Settings...
int dataPin = 2;
int clockPin = 3;
int pins = 20;
WS2801 strip = WS2801(pins, dataPin, clockPin);
boolean debug_mode = true;
unsigned int cycles = 0;
float waitFloat = (((60 / bpm) / (steps * 2)) * 1000);
int wait = (int) waitFloat;
int surgeLevel = 0;

// Required Functions...

void setup() {
  strip.begin();
  strip.show();
  
  // Debug.
  if(debug_mode) {
    Serial.begin(115200);
    Serial.println("Ready to send data.");
  }
}

void loop() {
  int frame = (cycles / 2) / steps;
  int i;
  for (i=0; i < steps*2; i++) {
    if (i >= steps) {
      // Run the color back.
      colorPicker(steps - (i - steps), frame);
    }
    else {
      // Run the color forward.
      colorPicker(i, frame);
    }
    cyclist(frame);
    strip.show();
    delay(wait);
  }
}

// Helper Functions...

// Pulse blueish for a while, then sprinkle in red, and finally bloom, then return.
// Purlse intensity as well. Add in color fluctuation if possible.

// Larger pattern.
void cyclist(int f) {
  // Base this timing on a pulse speed count.
  if ((f!=0) && f % surgeSpeed == 0) {
    report(5,5,5,String(surgeLevel));
    // Increase/decrease how many red are up.
    // Only when on the first cycle of a frame.
    if (cycles == f * (pins*2)) {
      if ((f % pins) >= (pins/2)) {
        surgeLevel--;
      }
      else {
        surgeLevel++;
      }
    }
  }

  int i;
  // Color the red pixels.
  for (i=0;i<=surgeLevel;i++) {
    strip.setPixelColor(i, Color(255, 0, 0));
  }

  cycles++;
}

// Pulse blue picker.
void colorPicker(int i, int f) {

  int red = ((abs(color1[0] - color2[0]) / steps) * i) + color1[0];
  int green = ((abs(color1[1] - color2[1]) / steps) * i) + color1[1];
  int blue = ((abs(color1[2] - color2[2]) / steps) * i) + color1[2];
  
  if (randomness) {
    switch (random(0,3)) {
      case 0: 
        red = red - random(0,randomness);
        break;
        
      case 1:
        green = green - random(0,randomness);
        break;
        
      case 2:
        blue = blue - random(0,randomness);
        break;
    }
  }
  
  colorWipe(Color(red, green, blue));
  
  if(debug_mode) {
    String msg = "<< << << #";
    msg += cycles;
    msg += " f:";
    msg += f;
    report(red, green, blue, msg);
  }

}

// Set the background color.
void colorWipe(uint32_t c) {
  int i;
  for (i=0; i < strip.numPixels(); i++) {   
    strip.setPixelColor(i, c);
  }
}

// Define a color.
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

// Debugger.
void report (int r, int g, int b, String msg) {
  if(debug_mode) {
    Serial.print("r: ");
    Serial.print(r);
    Serial.print("\t g: ");
    Serial.print(g);
    Serial.print("\t b: ");
    Serial.print(b);
    
    Serial.print("\t msg: ");
    Serial.print(msg);
  
    Serial.println();
  }
}
