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
    AutopilotSetting::plus();
    value+=100;
  }
  virtual void minus() {
    AutopilotSetting::minus();
    value-=100;
    if(value < 0) {
      value = 0;
    }
  }

  virtual void sendToGame() {
    Serial.print("A ");
    Serial.println(value);
  }

  void altitudeOn() {
    display->setBottomLeftIndicator("ALT");
  }

  void altitudeOff() {
    display->clearBottomLeftIndicator();
  }
};


#endif