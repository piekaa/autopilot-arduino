#ifndef HEADING_SETTING_H
#define HEADING_SETTING_H

#include "AutopilotSetting.h"
#include "Display.h"

class HeadingSetting : public AutopilotSetting {

  Display* display;
  volatile int heading;
  volatile int previousHeading;

  static void headingTaskEntry(void* param) {
    HeadingSetting* self = static_cast<HeadingSetting*>(param);
    self->headingUpdateLoop();
  }

  void headingUpdateLoop() {
    for(;;) {
      vTaskDelay(pdMS_TO_TICKS(100));

      if(heading != previousHeading) {
        display->showText(String(heading));
        previousHeading = heading;
      }
    }
  }

public:

  HeadingSetting(Display* display) {
    this->display = display;
    this->heading = 0;
    this->previousHeading = -1;

    xTaskCreatePinnedToCore(
      headingTaskEntry,
      "HeadingTask",
      4096,
      this,
      2,
      NULL,
      0
    );
  }

  virtual void plus() {
    heading++;
    if(heading > 359) {
      heading = 0;
    }
  }
  virtual void minus() {
    heading--;
    if(heading < 0) {
      heading = 359;
    }
  }

  int getHeading() const {
    return heading;
  }
};


#endif