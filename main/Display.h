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

  // Indicator state
  String leftIndicatorText = "";
  String rightIndicatorText = "";
  bool leftIndicatorEnabled = false;
  bool rightIndicatorEnabled = false;

  // Indicator layout constants
  static const uint8_t INDICATOR_TEXT_SIZE = 1;
  static const uint8_t INDICATOR_PADDING = 2;     // pixels around text
  static const uint8_t INDICATOR_MARGIN = 3;      // pixels from frame edge
  static const uint8_t INDICATOR_CHAR_WIDTH = 6;  // pixels per char at size 1
  static const uint8_t INDICATOR_CHAR_HEIGHT = 8; // pixels per char at size 1

  void renderFrame(bool draw) {
    uint16_t color = draw ? SSD1306_WHITE : SSD1306_BLACK;
    oledDisplay->drawRect(0, 0, 128, 64, color);
    oledDisplay->drawRect(1, 1, 126, 62, color);
  }

  void renderIndicator(String text, bool isRightAligned, bool draw) {
    if (text.length() == 0) return;

    uint16_t color = draw ? SSD1306_WHITE : SSD1306_BLACK;

    // Calculate text and border dimensions
    uint8_t textWidth = text.length() * INDICATOR_CHAR_WIDTH;
    uint8_t borderWidth = textWidth + (2 * INDICATOR_PADDING);
    uint8_t borderHeight = INDICATOR_CHAR_HEIGHT + (2 * INDICATOR_PADDING);

    // Calculate position (inside frame if enabled)
    uint8_t borderX, borderY;
    if (isRightAligned) {
      borderX = 128 - 2 - INDICATOR_MARGIN - borderWidth;  // Right aligned
    } else {
      borderX = 2 + INDICATOR_MARGIN;                      // Left aligned
    }
    borderY = 64 - 2 - INDICATOR_MARGIN - borderHeight;    // Bottom aligned

    // Draw border
    oledDisplay->drawRect(borderX, borderY, borderWidth, borderHeight, color);

    // Draw text inside border
    oledDisplay->setTextSize(INDICATOR_TEXT_SIZE);
    oledDisplay->setTextColor(color);
    oledDisplay->setCursor(borderX + INDICATOR_PADDING, borderY + INDICATOR_PADDING);
    oledDisplay->print(text);
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
    // Auto-render indicators if enabled
    if (leftIndicatorEnabled) {
      renderIndicator(leftIndicatorText, false, true);
    }
    if (rightIndicatorEnabled) {
      renderIndicator(rightIndicatorText, true, true);
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

  void setBottomLeftIndicator(String text) {
    ic2Multiplexer->selectAndLockChannel(channel);

    // Erase old indicator if it existed
    if (leftIndicatorEnabled && leftIndicatorText.length() > 0) {
      renderIndicator(leftIndicatorText, false, false);  // Draw in BLACK to erase
    }

    // Update state
    leftIndicatorText = text;
    leftIndicatorEnabled = text.length() > 0;

    // Draw new indicator if enabled
    if (leftIndicatorEnabled) {
      renderIndicator(leftIndicatorText, false, true);  // Draw in WHITE
    }

    oledDisplay->display();
    ic2Multiplexer->unlockChannel();
  }

  void setBottomRightIndicator(String text) {
    ic2Multiplexer->selectAndLockChannel(channel);

    // Erase old indicator if it existed
    if (rightIndicatorEnabled && rightIndicatorText.length() > 0) {
      renderIndicator(rightIndicatorText, true, false);  // Draw in BLACK to erase
    }

    // Update state
    rightIndicatorText = text;
    rightIndicatorEnabled = text.length() > 0;

    // Draw new indicator if enabled
    if (rightIndicatorEnabled) {
      renderIndicator(rightIndicatorText, true, true);  // Draw in WHITE
    }

    oledDisplay->display();
    ic2Multiplexer->unlockChannel();
  }

  void clearBottomLeftIndicator() {
    setBottomLeftIndicator("");
  }

  void clearBottomRightIndicator() {
    setBottomRightIndicator("");
  }
};

#endif