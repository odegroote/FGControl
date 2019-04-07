#ifndef __ENCODER_H__
#define __ENCODER_H__

class Encoder {
    /*
      wraps encoder setup and update functions in a class

      !!! NOTE : user must call the encoders update method from an
      interrupt function himself! i.e. user must attach an interrupt to the
      encoder pin A and call the encoder update method from within the
      interrupt

      uses Arduino pull-ups on A & B channel outputs
      turning on the pull-ups saves having to hook up resistors
      to the A & B channel outputs
  */
  public:

    // constructor : sets pins as inputs and turns on pullup resistors

    Encoder( int8_t PinA, int8_t PinB, int8_t buttonPin);

    void update ();
    long getPosition ();
    void setPosition(const long p);

    int readButton();


  private:

  long position;
  int8_t pin_a;
  int8_t pin_b;
  int8_t pin_button;

  int buttonState;      // the current reading from the input pin
  int lastButtonState;;   // the previous reading from the input pin
  // the following variables are unsigned longs because the time, measured in
  // milliseconds, will quickly become a bigger number than can be stored in an int.
  unsigned long lastDebounceTime;  // the last time the output pin was toggled
  unsigned long debounceDelay;    // the debounce time; increase if the output flickers
};

#endif // __ENCODER_H__
