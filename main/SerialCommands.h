#ifndef SERIAL_COMMANDS_H
#define SERIAL_COMMANDS_H

#include "HeadingSetting.h"
#include "SpeedSettings.h"
#include "AltitudeSettings.h"
#include "VerticalSpeedSettings.h"
#include "IOExpander.h"

class SerialCommands {

  HeadingSetting* headingSetting;
  SpeedSettings* speedSettings;
  AltitudeSettings* altitudeSettings;
  VerticalSpeedSettings* verticalSpeedSettings;
  IOExpander* ioExpander;

  static void commandsTaskEntry(void* param) {
    SerialCommands* self = static_cast<SerialCommands*>(param);
    self->serialLoop();
  }

  void serialLoop() {
    for (;;) {
      vTaskDelay(pdMS_TO_TICKS(10));
      if (Serial.available() > 0) {
        String commandType = Serial.readStringUntil(' ');
        if (commandType == "H") {
          int value = Serial.parseInt();
          headingSetting->setValue(value);
          Serial.readStringUntil('\n');
          continue;
        }

        if (commandType == "S") {
          int value = Serial.parseInt();
          speedSettings->setValue(value);
          Serial.readStringUntil('\n');
          continue;
        }

        if (commandType == "A") {
          int value = Serial.parseInt();
          altitudeSettings->setValue(value);
          Serial.readStringUntil('\n');
          continue;
        }

        if (commandType == "VS") {
          int value = Serial.parseInt();
          verticalSpeedSettings->setValue(value);
          Serial.readStringUntil('\n');
          continue;
        }

        if (commandType == "AP") {
          String apStatus = Serial.readStringUntil('\n');

          if(apStatus == "ON") {
            ioExpander->setLED(5, true);
          }

          if(apStatus == "OFF") {
            ioExpander->setLED(5, false);
          }
          continue;
        }


        if(commandType == "AP_HEADING") {
            String headingStatus = Serial.readStringUntil('\n');

          if(headingStatus == "ON") {
            headingSetting->headingOn();
          }

          if(headingStatus == "OFF") {
            headingSetting->headingOff();
          }
          continue;
        }

        if(commandType == "AP_LNAV") {
          String lnavStatus = Serial.readStringUntil('\n');

          if(lnavStatus == "ON") {
            headingSetting->navOn();
          }

          if(lnavStatus == "OFF") {
            headingSetting->navOff();
          }
          continue;
        }

        if(commandType == "AP_VNAV") {
          String vnavStatus = Serial.readStringUntil('\n');

          if(vnavStatus == "ON") {
            verticalSpeedSettings->vnavOn();
          }

          if(vnavStatus == "OFF") {
            verticalSpeedSettings->vnavOff();
          }
          continue;
        }

        if(commandType == "AP_SPEED") {
          String speedStatus = Serial.readStringUntil('\n');

          if(speedStatus == "ON") {
            speedSettings->speedOn();
          }

          if(speedStatus == "OFF") {
            speedSettings->speedOff();
          }
          continue;
        }

        if(commandType == "AP_ALTITUDE") {
          String altitudeStatus = Serial.readStringUntil('\n');

          if(altitudeStatus == "ON") {
            altitudeSettings->altitudeOn();
          }

          if(altitudeStatus == "OFF") {
            altitudeSettings->altitudeOff();
          }
          continue;
        }

        if(commandType == "AP_VS") {
          String vsStatus = Serial.readStringUntil('\n');

          if(vsStatus == "ON") {
            verticalSpeedSettings->vsOn();
          }

          if(vsStatus == "OFF") {
            verticalSpeedSettings->vsOff();
          }
          continue;
        }


        Serial.readStringUntil('\n');
      }
    }
  }

public:
  SerialCommands(HeadingSetting* headingSetting,
                 SpeedSettings* speedSettings,
                 AltitudeSettings* altitudeSettings,
                 VerticalSpeedSettings* verticalSpeedSettings,
                 IOExpander* ioExpander) {


    this->headingSetting = headingSetting;
    this->speedSettings = speedSettings;
    this->altitudeSettings = altitudeSettings;
    this->verticalSpeedSettings = verticalSpeedSettings;
    this->ioExpander = ioExpander;

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
