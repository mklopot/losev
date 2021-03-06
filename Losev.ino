#include <EEPROM.h>

/*
  "Adafruit Trinket"-based RGB LED light with a single control button.
  M.Klopot 2017-10-16 
 */


// set pin numbers:
const int buttonPin = 2; // Button wired to pin 2 and +5V    
const int led_r =  0;    // Red on pin 0, 220kOhm, which is too high for a red LED:)
const int led_g =  1;    // Green on pin 1, 220kOhm
const int led_b =  4;    // Blue on pin 4, which is also used for programming the Trinket, program before connecting this


    // Variation in brightness is calculated below as a fraction of 256, 
    // that is consistent with the usual input values to analogWrite().
    // Values are selected to work well in different light conditions:
    // noticeable, but not annoying.
    // Flicker here is pink noise, 1/(f^a). The 'a' exponent is set 
    // to 0.38, by trial and error, it controls how 'flickery' the flicker is. 
    // Try experimenting with these values to your liking!
const int baseBrightness = 130;
const int UndulationAmplitude = 30;

// variables
int buttonState = HIGH;         // variable for reading the pushbutton status
int lastButtonState = HIGH;
long lastButtonPress = 0;       // keep track of last press for de-bouncing 
int buttonDelay = 130;          // milliseconds

int current_color;

long now = 0;
float brightness;
float undulation;
int crackle_refresh = 34;       // This gives an update rate of about 30 Hz, but we can fuzz this number later
long next_crackle = 0;

long nextPink = 0;
int pinkRefresh = 95;
int pinkAmplitude = 25;
int pinkCounter = 0;
int oldRNG;
int newRNG;
int newZero;
int oldZero;
int pinkIndex;
int pinkValue;
// This is called the Voss-McCartney Pink Noise Algorithm
int pinkRNGs[] = {0, 0, 0, 0, 0, 0};
int pinkSequence[] = {1, 2, 1, 3,
                      1, 2, 1, 4,
                      1, 2, 1, 3,
                      1, 2, 1, 5,
                      1, 2, 1, 3,
                      1, 2, 1, 4,
                      1, 2, 1, 3,
                      1, 2, 1, 0};




struct Color {
  unsigned short int red;
  unsigned short int green;
  unsigned short int blue;
};

// color wheel: primary, seconday and tretiary colors. Plus white and black.
const struct Color red = {255, 0, 0};
const struct Color orange = {255, 25, 0};
const struct Color yellow = {255, 50, 0};
const struct Color chartreuse = {130, 200, 0};
const struct Color green = {0, 255, 0};
const struct Color springGreen = {0, 200, 90};  // Unused, looks too similar to true green in the current setup
const struct Color cyan = {0, 255, 255};
const struct Color azure = {0, 90, 255};
const struct Color blue = {0, 0, 255};
const struct Color violet = {90, 0, 255};
const struct Color magenta = {255, 0, 255};
const struct Color rose = {255, 0, 90};
const struct Color white = {255, 255, 200};  // should be {255,255,255}, do the math on the red resistor...
const struct Color off = {0, 0, 0};

struct Color *colors[] = {&orange, 
                          &yellow, 
                          &chartreuse, 
                          &green, 
                          &azure, 
                          &blue, 
                          &violet, 
                          &magenta, 
                          &rose, 
                          &red, 
                          &white, 
                          &off}; 

void setup() {   
  // initialize the pushbutton pin as an input with a pullup  
  pinMode(buttonPin, INPUT_PULLUP);

      // This section re-sets the color to what it was last time the device was unplugged
  current_color = EEPROM.read(0);  
  if (current_color < 0 or current_color > 11) {
    current_color = 0;
  }  
}

void loop() {
  // read the state of the pushbutton value:
  lastButtonState = buttonState;
  buttonState = digitalRead(buttonPin);
  now = millis();
  
  if (buttonState != lastButtonState and buttonState == LOW and (now - lastButtonPress > buttonDelay)) {     
    lastButtonPress = now;
    current_color++;
    if (current_color > 11) {
      current_color = 0;
    }
    EEPROM.write(0, current_color);    // Save the new color to EEPROM
  }
  
  if (now > nextPink) {
    nextPink += pinkRefresh;
    pinkCounter++;
    if (pinkCounter > 31) pinkCounter = 0;
    pinkIndex = pinkSequence[pinkCounter];
    if (pinkIndex > 0) {
      oldRNG = pinkRNGs[pinkIndex];
      newRNG = random(0, pinkAmplitude);
      pinkRNGs[pinkIndex] = newRNG;
      pinkValue += (newRNG - oldRNG);
    }
    oldZero = pinkRNGs[0];
    newZero = random(0, pinkAmplitude);
    pinkRNGs[0] = newZero;
    pinkValue += (newZero - oldZero);
  }
  
  if (now > next_crackle) {
    next_crackle += crackle_refresh;
    next_crackle -= random(0,20);  // The refresh rate can vary by 20 msec
    
    
    undulation = sin(now / 1256.0); // characteristic flicker at about 5 Hz
    
    // Brightness is a coeficient between 0 and 1, which we multiply 
    // by the RGB values for the current color,
    // to apply undulation and flicker.
    
    brightness = (baseBrightness + pinkValue + UndulationAmplitude * undulation) / 255;
    brightness = min(brightness,1);
    
    analogWrite(led_r, ceil(colors[current_color]->red * brightness));
    analogWrite(led_g, ceil(colors[current_color]->green * brightness));
    analogWrite(led_b, ceil(colors[current_color]->blue * brightness));
  }
}
