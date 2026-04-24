#ifndef SPEED_SETTING_H
#define SPEED_SETTING_H

#include "AutopilotSetting.h"
#include "Display.h"

class SpeedSettings : public AutopilotSetting {
public:

  int minValue = 0;
  int maxValue = 420;

  SpeedSettings(Display* display)
    : AutopilotSetting(display, "SpeedSettingTask") {}

  virtual void plus() {
    AutopilotSetting::plus();
    value++;
    if (value > 450) {
      value = 450;
    }
  }
  virtual void minus() {
    AutopilotSetting::minus();
    value--;
    if (value < 0) {
      value = 0;
    }
  }

  virtual bool isAtMinimum() {
    return value <= minValue;
  }

  virtual bool isAtMaximum() {
    return value >= maxValue;
  }

  virtual void sendToGame() {
    TCPSender* sender = TCPSender::getInstance();
    if (sender) {
      sender->sendLine("S " + String(value));
    }
  }

  void setGroundSpeed(int speed) {
    if (speed < 0) {
      display->clearBottomLeftIndicator();
    } else {
      display->setBottomLeftIndicator(String(speed));
    }
  }
};


#endif