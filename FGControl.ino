/*
 * FGControl.ino
*/

#include "channel.h"
#include "display.h"
#include "RotaryEncoder.h"

//--------------- Create an AD9833 object ---------------- 
// Note, SCK and MOSI must be connected to CLK and DAT pins on the AD9833 for SPI
// Defaults to 25MHz internal reference frequency
#define FNC_PIN1 9       // Can be any digital IO pin
Channel channel1(1, FNC_PIN1);
#define FNC_PIN2 10       // Can be any digital IO pin
Channel channel2(2, FNC_PIN2);
// contains the current channel operations should be preformd on
Channel* currentChannel = &channel1;

// create a displaylayout for 2 channels
LCD lcd(2);
// Let's put the current cursor position to channel-enable
CursorPosition currentCursorPosition = channel;
bool cursorOn = false;
unsigned long startTimeCursor=0;
void nextValue(Channel* chan, CursorPosition cursorPosition);
void previousValue(Channel* chan, CursorPosition cursorPosition);

Encoder encoder(2, A5, A0);

void doEncoder(){
    encoder.update();
}

void setup() {
  Serial.begin(9600); 

  lcd.begin();
  channel1.setSignal(SINE_WAVE,1000);
  lcd.updateChannelBox(&channel1);
  channel2.setSignal(SINE_WAVE,1000);
  lcd.updateChannelBox(&channel2);

  attachInterrupt( digitalPinToInterrupt(2), doEncoder, FALLING);

  Serial.println("initialized");
}

void loop() {
  if (encoder.readButton() == LOW) {
    startTimeCursor = millis();
    if (!cursorOn)
    {
      cursorOn = true;
    } else {
      lcd.setCursor(currentChannel, currentCursorPosition, false);
      CursorPosition newCursorPosition = lcd.nextCursorPosition(currentCursorPosition);
      if (newCursorPosition <= currentCursorPosition) {
        currentChannel = (currentChannel == &channel1 ? &channel2 : &channel1);
      }
      currentCursorPosition = newCursorPosition;
    }
    Serial.print("SetCursor:");Serial.print(currentCursorPosition);Serial.print(" ");Serial.println(cursorOn);
  } else if (cursorOn) {
    int rotaryDelta = encoder.getRotaryDelta();
    if (rotaryDelta!=0) {
      startTimeCursor = millis();
      if (rotaryDelta > 0) {
        nextValue(currentChannel, currentCursorPosition);
      } else {
        previousValue(currentChannel, currentCursorPosition);        
      }
    }
  }

  if (millis() - startTimeCursor > 10000) {
    cursorOn = false;
  }
  
  lcd.setCursor(currentChannel, currentCursorPosition, cursorOn);  
}

void nextValue(Channel* chan, CursorPosition cursorPosition)
{  
  switch(cursorPosition) {
    case channel:
      chan->toggleEnable();
    case shape:
      chan->setNextWaveForm();
      break;
    case freq1:
    case freq10:
    case freq100:
    case freq1000:
    case freq10000:
    case freq100000:
      chan->setFrequency(chan->getFrequency()+pow(10, currentCursorPosition - freq1));
      break;
  }
}

void previousValue(Channel* chan, CursorPosition cursorPosition)
{  
  switch(cursorPosition) {
    case channel:
      chan->toggleEnable();
    case shape:
      chan->setPreviousWaveForm();
      break;
    case freq1:
    case freq10:
    case freq100:
    case freq1000:
    case freq10000:
    case freq100000:
      chan->setFrequency(chan->getFrequency()-pow(10, currentCursorPosition - freq1));
      break;
  }
}
