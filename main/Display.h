#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IC2Multiplexer.h>

class Display {
  IC2Multiplexer* ic2Multiplexer;
  uint8_t channel;
  Adafruit_SSD1306* oledDisplay;
  bool frameEnabled = false;

  void renderFrame(bool draw) {
    uint16_t color = draw ? SSD1306_WHITE : SSD1306_BLACK;
    oledDisplay->drawRect(0, 0, 128, 64, color);
    oledDisplay->drawRect(1, 1, 126, 62, color);
  }

public:
  Display(IC2Multiplexer* ic2Multiplexer, uint8_t channel) {
    this->ic2Multiplexer = ic2Multiplexer;
    this->channel = channel;
    this->oledDisplay = new Adafruit_SSD1306(128, 64, &Wire1, -1);

    ic2Multiplexer->selectAndLockChannel(channel);
    bool displayInitialized = oledDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    if (displayInitialized) {
      oledDisplay->clearDisplay();
      oledDisplay->println("display ready");
      oledDisplay->display();
    } else {
      Serial.println("X Failed to initialize display");
    }
    ic2Multiplexer->unlockChannel();
  }

  void showText(String text) {
    ic2Multiplexer->selectAndLockChannel(channel);
    oledDisplay->clearDisplay();
    oledDisplay->setTextSize(2);
    oledDisplay->setTextColor(SSD1306_WHITE);
    oledDisplay->setCursor(10, 25);
    oledDisplay->println(text);
    if (frameEnabled) {
      renderFrame(true);
    }
    oledDisplay->display();
    ic2Multiplexer->unlockChannel();
  }

  void drawFrame() {
    ic2Multiplexer->selectAndLockChannel(channel);
    frameEnabled = true;
    renderFrame(true);
    oledDisplay->display();
    ic2Multiplexer->unlockChannel();
  }

  void removeFrame() {
    ic2Multiplexer->selectAndLockChannel(channel);
    frameEnabled = false;
    renderFrame(false);
    oledDisplay->display();
    ic2Multiplexer->unlockChannel();
  }
};

#endif