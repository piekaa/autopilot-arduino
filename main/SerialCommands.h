#ifndef SERIAL_COMMANDS_H
#define SERIAL_COMMANDS_H

#include "HeadingSetting.h"
#include "SpeedSettings.h"
#include "AltitudeSettings.h"
#include "VerticalSpeedSettings.h"

class SerialCommands {

 HeadingSetting* headingSetting;
  SpeedSettings* speedSettings;
  AltitudeSettings* altitudeSettings;
  VerticalSpeedSettings* verticalSpeedSettings;

  static void commandsTaskEntry(void* param) {
    SerialCommands* self = static_cast<SerialCommands*>(param);
    self->serialLoop();
  }

  void serialLoop() {
    for (;;) {
      vTaskDelay(pdMS_TO_TICKS(100));
      if (Serial.available() > 0) {
        String commandType = Serial.readStringUntil(' ');
        if (commandType == "H") {
          int value = Serial.parseInt();
          headingSetting->setValue(value);
        }
        Serial.readStringUntil('\n');
      }
    }
  }

public:
  SerialCommands( HeadingSetting* headingSetting,
  SpeedSettings* speedSettings,
  AltitudeSettings* altitudeSettings,
  VerticalSpeedSettings* verticalSpeedSettings) {
   

  this->headingSetting = headingSetting;
  this->speedSettings= speedSettings;
  this->altitudeSettings = altitudeSettings;
  this->verticalSpeedSettings = verticalSpeedSettings;

    xTaskCreatePinnedToCore(
      commandsTaskEntry,
      "CommandsTask",
      8192,
      this,
      2,
      NULL,
      0);
  }
};

#endif
