#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "channel.h"

#ifndef _LCD
  #define _LCD

#define NR_CURSORPOSITIONS 8
typedef enum CursorPosition {channel, shape, freq1, freq10, freq100, freq1000, freq10000, freq100000};


class LCD {
  public:
  LCD(int nrOfChannels);
  void begin();
  void updateChannelBox(Channel* channel);
  void setCursor(Channel* channel, CursorPosition cursorPosition, bool on);
  CursorPosition  nextCursorPosition(CursorPosition cursorPosition);
  
  private:
  
  void setupChannelBox(int channelNr);
  void setChannelCursor(Channel* chanel, bool on);
  void setWaveformCursor(Channel* channel, bool on);
  void setFrequencyCursor(Channel* channel, int index, bool on);
  void showEnabled(Channel* channel);
  
  Adafruit_PCD8544* display;
};
#endif
