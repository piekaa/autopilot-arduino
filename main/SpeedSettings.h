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
    if(value > 450) {
      value = 450;
    }
  }
  virtual void minus() {
    value--;
    if(value < 0) {
      value = 0;
    }
  }
};


#endif