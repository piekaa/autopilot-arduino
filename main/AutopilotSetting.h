#ifndef AUTOPILOT_SETTING_H
#define AUTOPILOT_SETTING_H

#include <Display.h>

class AutopilotSetting {
  Display* display;
  volatile int previousValue;


  static void taskEntry(void* param) {
    AutopilotSetting* self = static_cast<AutopilotSetting*>(param);
    self->updateLoop();
  }

  void updateLoop() {
    for (;;) {
      vTaskDelay(pdMS_TO_TICKS(100));

      if (value != previousValue) {
        display->showText(String(value));
        previousValue = value;
      }
    }
  }


protected:
  volatile int value;


public:

  AutopilotSetting(Display* display, const char* taskName) {
    this->display = display;
    this->value = 0;
    this->previousValue = -1;

    xTaskCreatePinnedToCore(
      taskEntry,
      taskName,
      4096,
      this,
      2,
      NULL,
      0);
  }

  virtual void plus() {
    Serial.println("X [plus] Implement me in child class");
  }
  virtual void minus() {
    Serial.println("X [minus] Implement me in child class");
  }
};


#endif