#ifndef AUTOPILOT_SETTING_H
#define AUTOPILOT_SETTING_H

#include <Display.h>

class AutopilotSetting {
  
  volatile int previousValue;

  volatile bool active = false;
  volatile bool previousActive = false;

  static void taskEntry(void* param) {
    AutopilotSetting* self = static_cast<AutopilotSetting*>(param);
    self->updateLoop();
  }

  void updateLoop() {
    for (;;) {
      vTaskDelay(pdMS_TO_TICKS(100));

      lock--;

      if (active != previousActive) {

        if (active) {
          display->drawFrame();
        } else {
          display->removeFrame();
        }

        previousActive = active;
      }

      if (value != previousValue) {
        display->showText(String(value));
        this->sendToGame();
        previousValue = value;
      }
    }
  }


protected:
  volatile int value;
  int lock = 0;
  Display* display;

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

  virtual void sendToGame() {
    Serial.println("X [sendToGame] Implement me in child class");
  }

  void activate() {
    active = true;
  }

  void deactivate() {
    active = false;
  }

  void setValue(int value) {
    if(lock > 0) {
      return;
    }
    this->value = value;
  }
};


#endif