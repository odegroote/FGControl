#ifndef _CHANNEL
 #define _CHANNEL
 
#include <AD9833.h>     // Include the library

class Channel {
  public:
  Channel(int channelNr, int8_t pin);
  Channel(int channelNr, AD9833* ad9833);
  void reset();
  bool isEnabled();
  void enable(bool flag);
  void toggleEnable();
  void setSignal(int waveform, unsigned long frequency);
  int getWaveForm();
  void setWaveForm(int waveform);
  void setNextWaveForm();
  void setPreviousWaveForm();
  unsigned long getFrequency();
  void setFrequency(unsigned long frequency);
  int getChannelNr();
  
  private:
  int channelNr;
  bool enabled;
  int pin;
  unsigned long frequency;
  int waveform;
  AD9833* ad9833;
};
#endif
