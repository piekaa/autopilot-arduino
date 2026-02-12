#ifndef HEADING_SETTING_H
#define HEADING_SETTING_H

#include "AutopilotSetting.h"
#include "Display.h"

class HeadingSetting : public AutopilotSetting {
public:
  HeadingSetting(Display* display)
    : AutopilotSetting(display, "HeadingSettingTask") 
  {}

  virtual void plus() {
    value++;
    if(value > 359) {
      value = 0;
    }
  }
  virtual void minus() {
    value--;
    if(value < 0) {
      value = 359;
    }
  }
};


#endif