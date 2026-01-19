#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display {
  IC2Multiplexer* ic2Multiplexer;
  uint8_t channel;
  Adafruit_SSD1306* oledDisplay;
public:
  Display(IC2Multiplexer* ic2Multiplexer, uint8_t channel) {
    this->ic2Multiplexer = ic2Multiplexer;
    this->channel = channel;
    this->oledDisplay = new Adafruit_SSD1306(128, 64, &Wire1, -1);

    ic2Multiplexer->selectChannel(channel);
    oledDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  }

  void showText(String text) {
    ic2Multiplexer->selectChannel(channel);

    oledDisplay->clearDisplay();
    oledDisplay->setTextSize(2);  // Set text size (1 is default)
    oledDisplay->setTextColor(SSD1306_WHITE);
    oledDisplay->setCursor(0, 0); // Set starting position

    // Note: Adafruit library uses print()
    oledDisplay->println(text);

    oledDisplay->display(); // Show the content
  }
};

#endif