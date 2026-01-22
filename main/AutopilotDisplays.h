#ifndef AUTOPILOT_DISPLAYS_H
#define AUTOPILOT_DISPLAYS_H

#include "Display.h"

class AutopilotDisplays {
  IC2Multiplexer* ic2Multiplexer;

   Display* heading;
   Display* speed;
   Display* altitude;
   Display* verticalSpeed;

  static void displayTaskEntry(void* param) {
    AutopilotDisplays* self = static_cast<AutopilotDisplays*>(param);
    self->displayLoop();
  }

public:
  AutopilotDisplays(IC2Multiplexer* ic2Multiplexer) {
    this->ic2Multiplexer = ic2Multiplexer;

    heading = new Display(ic2Multiplexer, 0);
    speed = new Display(ic2Multiplexer, 7);
    altitude = new Display(ic2Multiplexer, 6);
    verticalSpeed = new Display(ic2Multiplexer, 5);

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

  void displayLoop() {
      for(;;) {
        heading->showText("Heading");
        speed->showText("Speed");
        altitude->showText("Altitude");
        verticalSpeed->showText("VerticalSpeed");
      }
  }
};

#endif