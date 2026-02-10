#ifndef AUTOPILOT_SETTING_H
#define AUTOPILOT_SETTING_H

class AutopilotSetting {

public:
  virtual void plus() {
    Serial.println("X [plus] Implement me in child class");
  }
  virtual void minus() {
    Serial.println("X [minus] Implement me in child class");
  }
};


#endif