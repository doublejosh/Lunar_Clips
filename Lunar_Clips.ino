#include "SPI.h"
#include "WS2801.h"

// Visual Settings...
int color1[] = {40, 50, 120};
int color2[] = {255, 255, 255};
float bpm = 70; // beats per minute.
float cph = 120; // cycles per hour.
float steps = 20; // steps between colors (bg).
int randomness = 0; // color steps to jump around while fading (bg).

// System Settings...
int dataPin = 2;
int clockPin = 3;
WS2801 strip = WS2801(20, dataPin, clockPin);
boolean debug_mode = true;
unsigned int cycles = 0;
float waitFloat = (((60 / bpm) / (steps * 2)) * 1000);
int wait = (int) waitFloat;

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
  
  int i;
  for (i=0; i < steps*2; i++) {
    if (i >= steps) {
      colorPicker(steps - (i - steps));
    }
    else {
      colorPicker(i);
    }
    
    cyclist();
    
    strip.show();
    delay(wait);

  }
}

// Helper Functions...

// Pulse blueish for a while, then sprinkle in red, and finally bloom, then return.
// Purlse intensity as well. Add in color fluctuation if possible.

// Larger pattern.
void cyclist() {
  if (cycles % 120 < 8) {
    colorWipe(Color(255, 0, 0));
  }
  else {
      
  }
  cycles++;
}

// Pulse blue picker.
void colorPicker(int i) {

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
    msg += " w:";
    msg += wait;
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
