#include "channel.h"

Channel::Channel(int channelNr, int8_t pin)
{
  this->channelNr = channelNr;
  this->waveform=TRIANGLE_WAVE;
  this->frequency=1000;
  this->enabled = false;
  this->ad9833 = new AD9833(pin);       // Defaults to 25MHz internal reference frequency
}

Channel::Channel(int channelNr,AD9833* ad9833)
{
  this->channelNr = channelNr;
  this->waveform=TRIANGLE_WAVE;
  this->frequency=1000;
  this->enabled = false;
  this->ad9833 = ad9833;       // Defaults to 25MHz internal reference frequency
}

void Channel::reset()
{
  ad9833->Reset();  
}

bool Channel::isEnabled()
{
  return this->enabled;
}

void Channel::enable(bool flag)
{
  this->enabled = flag;
  ad9833->EnableOutput(flag);
}

void Channel::toggleEnable()
{
  this->enable(!this->isEnabled());
}

// Apply a 1000 Hz sine wave using REG0 (register set 0). There are two register sets,
  // REG0 and REG1. 
  // Each one can be programmed for:
  //   Signal type - SINE_WAVE, TRIANGLE_WAVE, SQUARE_WAVE, and HALF_SQUARE_WAVE
  //   Frequency - 0 to 12.5 MHz
  //   Phase - 0 to 360 degress (this is only useful if it is 'relative' to some other signal
  //           such as the phase difference between REG0 and REG1).
  // In ApplySignal, if Phase is not given, it defaults to 0.
void Channel::setSignal(int waveform, unsigned long frequency)
{
  this->waveform=waveform;
  this->frequency = frequency;  
  ad9833->ApplySignal(waveform,REG0,frequency);
}

int Channel::getChannelNr()
{
  return this->channelNr;
}

int Channel::getWaveForm()
{
  return this->waveform;
}

void Channel::setWaveForm(int waveform)
{
  this->waveform = waveform;
  this->setSignal(waveform, frequency);
}

void Channel::setNextWaveForm()
{
  switch (getWaveForm()){
    case SINE_WAVE:
      setWaveForm(TRIANGLE_WAVE);
      break;
    case TRIANGLE_WAVE:
      setWaveForm(SQUARE_WAVE);
      break;
    case SQUARE_WAVE:
      setWaveForm(HALF_SQUARE_WAVE);
      break;
    case HALF_SQUARE_WAVE:
      setWaveForm(SINE_WAVE);
      break;
  }  
}

void Channel::setPreviousWaveForm()
{
  switch (getWaveForm()){
    case SINE_WAVE:
      setWaveForm(HALF_SQUARE_WAVE);
      break;
    case TRIANGLE_WAVE:
      setWaveForm(SINE_WAVE);
      break;
    case SQUARE_WAVE:
      setWaveForm(TRIANGLE_WAVE);
      break;
    case HALF_SQUARE_WAVE:
      setWaveForm(SQUARE_WAVE);
      break;
  }
}

unsigned long Channel::getFrequency()
{
  return this->frequency;
}

void Channel::setFrequency(unsigned long frequency)
{
  this->frequency = frequency;
  this->setSignal(waveform, frequency);
}
