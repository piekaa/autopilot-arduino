#ifndef VERTICAL_SPEED_SETTING_H
#define VERTICAL_SPEED_SETTING_H

#include "AutopilotSetting.h"
#include "Display.h"

class VerticalSpeedSettings : public AutopilotSetting {
public:
  VerticalSpeedSettings(Display* display)
    : AutopilotSetting(display, "VerticalSpeedSettingTask") 
  {}

  virtual void plus() {
    value+=100;
    lock = 10;
  }
  virtual void minus() {
    value-=100;
    lock = 10;
  }

  virtual void sendToGame() {
    Serial.print("VS ");
    Serial.println(value);
  }
};


#endif