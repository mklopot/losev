#include <EEPROM.h>

/*
  "Adafruit Trinket"-based RGB LED light with a single control button.
  M.Klopot 2017-10-16 
 */


// set pin numbers:
const int buttonPin = 2;     
const int led_r =  0;
const int led_g =  1;
const int led_b =  4;


// variables
int buttonState = HIGH;         // variable for reading the pushbutton status
int lastButtonState = HIGH;
long lastButtonPress = 0;
int buttonDelay = 250; // milliseconds

long now = 0;
int crackle_refresh = 82;
long next_crackle = 0;

int eeprom = EEPROM.read(0)
if (eeprom >= 0 and eeprom <= 11) {
  int current_color = eeprom;
}
else {
  int current_color = 0;
}

struct Color {
  unsigned short int red;
  unsigned short int green;
  unsigned short int blue;
};

void lightUp(struct Color *color);

// color wheel: primary, seconday and tretiary colors
const struct Color red = {255, 0, 0};
const struct Color orange = {255, 25, 0};
const struct Color yellow = {255, 50, 0};
const struct Color chartreuse = {130, 200, 0};
const struct Color green = {0, 255, 0};
const struct Color springGreen = {0, 200, 90};
const struct Color cyan = {0, 255, 255};
const struct Color azure = {0, 90, 255};
const struct Color blue = {0, 0, 255};
const struct Color violet = {90, 0, 255};
const struct Color magenta = {255, 0, 255};
const struct Color rose = {255, 0, 90};
const struct Color white = {255, 255, 230};  // should be {255,255,255}, do the math on the red resistor...
const struct Color off = {0, 0, 0};

struct Color *colors[] = {&orange, &yellow, &chartreuse, &green, &azure, &blue, &violet, &magenta, &rose, &red, &white, &off}; 

void setup() {
  // initialize the LED pin as an output:    
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);     
}

void loop(){
  // read the state of the pushbutton value:

  lastButtonState = buttonState;
  buttonState = digitalRead(buttonPin);
  now = millis();
  
  if (buttonState != lastButtonState and buttonState == LOW and (now - lasBUttontPress > button Delay)) {     
    lastButtonPress = now;
    current_color++;
    if (current_color > 11) {
      current_color = 0;
    }
    EEPROM.write(0, current_color);
  }

  if (now > next_crackle) {
    next_crackle += crackle_refresh - random(0,30);
    float undulation = sin(millis() / 5000.0); 
    float undulation2 = sin(millis() / 1000.0); 
    float brightness = (130 + 40 * undulation + 10 * undulation2 + 75 / (pow(random(1,256), 0.38))) / 256;
    analogWrite(led_r, ceil(colors[current_color]->red * brightness));
    analogWrite(led_g, ceil(colors[current_color]->green * brightness));
    analogWrite(led_b, ceil(colors[current_color]->blue * brightness));
  }
}
