#include "display.h"

LCD::LCD(int nrChannels)
{  
// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
  display = new Adafruit_PCD8544(7, 6, 5, 4, 3);
  this->nrChannels = nrChannels;
  this->channels = new Channel*[nrChannels];
  this->currentChannel = 0;

  // Let's put the current cursor position to channel-enable
  this->currentCursorPosition = channel;
}

void LCD::begin()
{
   display->begin();
  // you can change the contrast around to adapt the display
  // for the best viewing!
  display->clearDisplay();
  display->setContrast(60);
  display->setTextWrap(false);

  for(int i=0; i < nrChannels; i++) {
    this->setupChannelBox(i);
  }

  display->display();
}

void LCD::setupChannelBox(int channelNr)
{
  int verticalOffset = channelNr * (display->height()/2);
  display->drawRect(0, verticalOffset, display->width(), display->height()/2, BLACK);
  display->setCursor(2,2+verticalOffset);
  display->print("S:");
  display->setCursor(68,2+verticalOffset);
  display->print("C");
  display->print(channelNr);
  display->setCursor(3,15+verticalOffset);
  display->print("F:         Hz");  
}

void LCD::updateChannelBox(Channel* channel)
{
  channels[channel->getChannelNr()-1] = channel;
  this->showEnabled(channel);
  this->setWaveformCursor(channel, false);
  this->setFrequencyCursor(channel, 0, false);
  display->display();
}

void LCD::setCursor(bool on)
{
  Channel* chan = channels[currentChannel];
  switch(currentCursorPosition) {
    case channel:
      this->setChannelCursor(chan, on);
      this->showEnabled(chan);
      break;
    case shape:
      this->setWaveformCursor(chan, on);
      break;
    default:
      this->setFrequencyCursor(chan, currentCursorPosition-freq1, on);
      break;
  }
  display->display();
}

CursorPosition LCD::getCurrentCursorPosition()
{
  return currentCursorPosition;
}

Channel* LCD::nextCursorPosition()
{
  setCursor(false);

  CursorPosition newCursorPosition = (currentCursorPosition + 1) % NR_CURSORPOSITIONS;
  if (newCursorPosition <= currentCursorPosition) {
      currentChannel = (currentChannel + 1) % nrChannels;
  }
  currentCursorPosition = newCursorPosition;

  setCursor(true);
  
  return channels[currentChannel];
}

void LCD::setChannelCursor(Channel* channel, bool on)
{
  int channelNr = channel->getChannelNr();
  int verticalOffset = (channelNr-1) * (display->height()/2);
  display->fillRect(68,2+verticalOffset, 14, 8, WHITE); //Clear region
  display->setCursor(68,2+verticalOffset);
  display->print("C");
  if (on) {
    display->setTextColor(WHITE, BLACK);
  } else {
    display->setTextColor(BLACK, WHITE);    
  }
  display->print(channelNr);
  display->setTextColor(BLACK, WHITE);  
}

void LCD::showEnabled(Channel* channel)
{
  int channelNr = channel->getChannelNr();
  int verticalOffset = (channelNr-1) * (display->height()/2);
  display->fillCircle(80,5+verticalOffset,1, channel->isEnabled() ? BLACK : WHITE);
  display->display();
}

const char* waveFormName(int waveForm)
{
  switch(waveForm) {
    case SINE_WAVE:     return "SINE";
    case TRIANGLE_WAVE: return "TRIANGLE";
    case SQUARE_WAVE:   return "BLOCK";
    case HALF_SQUARE_WAVE:   return "H-BLOCK";
  }  
}

void LCD::setWaveformCursor(Channel* channel, bool on)
{
  int verticalOffset = (channel->getChannelNr()-1) * (display->height()/2);
  display->fillRect(14,2+verticalOffset, 50, 8, WHITE); //Clear region
  display->setCursor(14,2+verticalOffset);
  if (on) {
    display->setTextColor(WHITE, BLACK);
  } else {
    display->setTextColor(BLACK, WHITE); 
  }
  display->print(waveFormName(channel->getWaveForm()));
  display->setTextColor(BLACK, WHITE);    
}

void LCD::setFrequencyCursor(Channel* channel, int index, bool on)
{
  char freqbuffer[8];
  int verticalOffset = (channel->getChannelNr()-1) * (display->height()/2);
  display->fillRect(14,15  + verticalOffset, 50, 8, WHITE);
  display->setCursor(14,15  + verticalOffset);
  sprintf(freqbuffer, "%07ld", channel->getFrequency());
  for (int j=0; j<7; j++) {
    if (on && 6-index == j) {
      display->setTextColor(WHITE, BLACK);
    }
    display->print(freqbuffer[j]);
    display->setTextColor(BLACK, WHITE); 
  }
  display->display();  
}

