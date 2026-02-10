#ifndef AUTOPILOT_DISPLAYS_H
#define AUTOPILOT_DISPLAYS_H

#include "Display.h"

struct DisplayMsg {
  int heading, speed, altitude, verticalSpeed;
};

class AutopilotDisplays {
  IC2Multiplexer* ic2Multiplexer;

   Display* heading;
   Display* speed;
   Display* altitude;
   Display* verticalSpeed;

   QueueHandle_t displayQueue = nullptr;

  static void displayTaskEntry(void* param) {
    AutopilotDisplays* self = static_cast<AutopilotDisplays*>(param);
    self->displayLoop();
  }

   void displayLoop() {
    DisplayMsg msg{};

      for(;;) {
        if( xQueueReceive(displayQueue, &msg, portMAX_DELAY) == pdPASS) {
          heading->showText(String(msg.heading));
          speed->showText(String(msg.speed));
          altitude->showText(String(msg.altitude));
          verticalSpeed->showText(String(msg.verticalSpeed));
        }
      }
  }

public:
  AutopilotDisplays(IC2Multiplexer* ic2Multiplexer, SemaphoreHandle_t i2cMutex) {
    this->ic2Multiplexer = ic2Multiplexer;

    heading = new Display(ic2Multiplexer, 0, i2cMutex);
    speed = new Display(ic2Multiplexer, 7, i2cMutex);
    altitude = new Display(ic2Multiplexer, 6, i2cMutex);
    verticalSpeed = new Display(ic2Multiplexer, 5, i2cMutex);

    displayQueue = xQueueCreate(10, sizeof(DisplayMsg));

     xTaskCreatePinnedToCore(
    displayTaskEntry,        
    "DisplayTask",
    8192,                    
    this,                    
    3,                       
    NULL, 
    0                        
  );

  }

  void showValues(int heading, int speed, int altitude, int verticalSpeed) {
    DisplayMsg msg;
    msg.heading = heading;
    msg.speed = speed;
    msg.altitude = altitude;
    msg.verticalSpeed = verticalSpeed;

    xQueueSend(displayQueue, &msg, 0);
  }
};

#endif