#ifndef HEADING_SETTING_H
#define HEADING_SETTING_H

#include "AutopilotSetting.h"
#include "Display.h"

class HeadingSetting : public AutopilotSetting {
public:
  HeadingSetting(Display* display)
    : AutopilotSetting(display, "HeadingSettingTask") {}

  virtual void plus() {
    AutopilotSetting::plus();
    value++;
    if (value > 359) {
      value = 0;
    }
  }
  virtual void minus() {
    AutopilotSetting::minus();
    value--;
    if (value < 0) {
      value = 359;
    }
  }

  virtual void sendToGame() {
    Serial.print("H ");
    Serial.println(value);
  }

  void headingOn() {
    display->setBottomLeftIndicator("HDG");
  }

  void headingOff() {
    display->clearBottomLeftIndicator();
  }

  void navOn() {
    display->setBottomRightIndicator("NAV");
  }

  void navOff() {
    display->clearBottomRightIndicator();
  }
};


#endif