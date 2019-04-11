/*
 * FGControl.ino
*/

#include "channel.h"
#include "display.h"
#include "RotaryEncoder.h"

// create 2 ad9833 channel objects
#define FNC_PIN1 9       // Can be any digital IO pin
Channel channel1(1, FNC_PIN1);
#define FNC_PIN2 10       // Can be any digital IO pin
Channel channel2(2, FNC_PIN2);
// contains the current channel operations should be performed on
Channel* currentChannel = &channel1;

// create a displaylayout for the 2 channels
LCD lcd(2);

bool cursorOn = false;
unsigned long startTimeCursor=0;
void updateValue(Channel* chan, bool up);

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
      currentChannel = lcd.nextCursorPosition();
    }
  } else if (cursorOn) {
    int rotaryDelta = encoder.getRotaryDelta();
    if (rotaryDelta!=0) {
      startTimeCursor = millis();
      updateValue(currentChannel, rotaryDelta > 0);
    }
  }

  if (millis() - startTimeCursor > 10000) {
    cursorOn = false;
  }
  
  lcd.setCursor(cursorOn);  
}

void updateValue(Channel* chan, bool up)
{  
  CursorPosition cursorPosition = lcd.getCurrentCursorPosition();
  switch(cursorPosition) {
    case channel:
      chan->toggleEnable();
      break;
    case shape:
      if (up)
        chan->setNextWaveForm();
      else
        chan->setPreviousWaveForm();
      break;
    case freq1:
    case freq10:
    case freq100:
    case freq1000:
    case freq10000:
    case freq100000:
      chan->setFrequency(chan->getFrequency()+(up ? 1 : -1) * pow(10, cursorPosition - freq1));
      break;
  }
}
