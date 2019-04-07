#include <Arduino.h>
#include "RotaryEncoder.h"

Encoder::Encoder( int8_t PinA, int8_t PinB, int8_t buttonPin) : pin_a ( PinA), pin_b( PinB ), pin_button( buttonPin ) {
  // set pin a and b to be input
  pinMode(pin_a, INPUT_PULLUP);
  pinMode(pin_b, INPUT_PULLUP);

  pinMode(pin_button, INPUT_PULLUP);

  lastButtonState = LOW;   // the previous reading from the input pin
  // the following variables are unsigned longs because the time, measured in
  // milliseconds, will quickly become a bigger number than can be stored in an int.
  lastDebounceTime = 0;  // the last time the output pin was toggled
  debounceDelay = 50;    // the debounce time; increase if the output flickers
};

void Encoder::update() {
  digitalRead(pin_b) ? position++ : position--;
};

long Encoder::getPosition () {
      return position;
};

void Encoder::setPosition ( const long p) {
      position = p;
};

int Encoder::readButton()
{
 // read the state of the switch into a local variable:
  int reading = digitalRead(pin_button);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } else if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;

  return buttonState;    
}

