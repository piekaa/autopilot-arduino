#ifndef ALTITUDE_SETTING_H
#define ALTITUDE_SETTING_H

#include "AutopilotSetting.h"
#include "Display.h"

class AltitudeSettings : public AutopilotSetting {
public:
  AltitudeSettings(Display* display)
    : AutopilotSetting(display, "AltitudeSettingTask") 
  {}

  virtual void plus() {
    value+=100;
  }
  virtual void minus() {
    value-=100;
    if(value < 0) {
      value = 0;
    }
  }

  virtual void sendToGame() {
    Serial.print("A ");
    Serial.println(value);
  }
};


#endif