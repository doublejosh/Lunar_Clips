#include "SPI.h"
#include "WS2801.h"

// Visual Settings...
int color1[] = {40, 50, 120};
int color2[] = {255, 255, 255};
float bpm = 70; // beats per minute.
int surgeSpeed = 1; // surge growth (in pulses).
float steps = 20; // steps between colors (bg).
int randomness = 0; // color steps to jump around while fading (bg).
int surgeHold[2][1] = {0, 20};  // Stay surge or blank color for a while.
surgeHold[0][0] = 5;
surgeHold[1][0] = 10;

//int surgeColor1[] = {8, 173, 35};
//int surgeColor2[] = {2, 102, 200};
int surgeColor2[] = {255, 10, 18};
int surgeColor1[] = {200, 0, 0};

// System Settings...
int dataPin = 2;
int clockPin = 3;
float pins = 20;
WS2801 strip = WS2801(pins, dataPin, clockPin);
boolean debug_mode = true;
unsigned int cycles = 0;
float waitFloat = (((60 / bpm) / (steps * 2)) * 1000);
int wait = (int) waitFloat;
float surgeLevel = 0;

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
  if (f % surgeSpeed == 0) {
    //report(5,5,5,String(surgeLevel));
    // Increase/decrease how many red are up.
    // Only when on the first cycle of a frame.
    if (cycles == (f * ((int) pins * 2))) {
      if ((f % ((int) pins * 2)) >= (int) pins) {
        surgeLevel--;
      }
      else {
        surgeLevel++;
      }
    }
  }
  
  if 
  
  // Pick the surge color.  
  float red = (((surgeColor1[0] - surgeColor2[0]) / pins) * surgeLevel) + surgeColor2[0];
  float green = (((surgeColor1[1] - surgeColor2[1]) / pins) * surgeLevel) + surgeColor2[1];
  float blue = (((surgeColor1[2] - surgeColor2[2]) / pins) * surgeLevel) + surgeColor2[2];
  //if(red > 255) { red = 255; }
  //if(green > 255) { red = 255; }
  //if(blue > 255) { red = 255; }
  uint32_t c = Color((int) red, (int) green, (int) blue);

  if(debug_mode) {
    String msg = " #";
    msg += cycles;
    msg += " f:";
    msg += f;
    msg += " S:";
    msg += (int) surgeLevel;
    report(red, green, blue, msg);
  }

  int i;
  // Color the red pixels.
  for (i=0;i<=(surgeLevel-1);i++) {
    strip.setPixelColor(i, c);
  }
  
  if (cycles < 65534) {
    cycles++;
  }
  else {
    cycles = 0;
  }
  
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
  
  /*
  if(debug_mode) {
    String msg = "<< << << #";
    msg += cycles;
    msg += " f:";
    msg += f;
    report(red, green, blue, msg);
  }
  */

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
