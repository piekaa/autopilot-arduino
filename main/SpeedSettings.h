#ifndef SPEED_SETTING_H
#define SPEED_SETTING_H

#include "AutopilotSetting.h"
#include "Display.h"

class SpeedSettings : public AutopilotSetting {
public:
  SpeedSettings(Display* display)
    : AutopilotSetting(display, "SpeedSettingTask") 
  {}

  virtual void plus() {
    value++;
    lock = 10;
    if(value > 450) {
      value = 450;
    }
  }
  virtual void minus() {
    value--;
    lock = 10;
    if(value < 0) {
      value = 0;
    }
  }

  virtual void sendToGame() {
    Serial.print("S ");
    Serial.println(value);
  }
};


#endif