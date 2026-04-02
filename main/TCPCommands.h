#ifndef TCP_COMMANDS_H
#define TCP_COMMANDS_H

#include <WiFi.h>
#include "HeadingSetting.h"
#include "SpeedSettings.h"
#include "AltitudeSettings.h"
#include "VerticalSpeedSettings.h"
#include "IOExpander.h"
#include "TCPSender.h"
#include "esp_task_wdt.h"

// WiFi credentials - change these to your network
const char* WIFI_SSID = "iwaniuk";
const char* WIFI_PASSWORD = "aaaa1234";

// Server connection settings
const char* SERVER_IP = "192.168.50.137";
const uint16_t SERVER_PORT = 5000;

class TCPCommands {

  HeadingSetting* headingSetting;
  SpeedSettings* speedSettings;
  AltitudeSettings* altitudeSettings;
  VerticalSpeedSettings* verticalSpeedSettings;
  IOExpander* ioExpander;

  String buffer = "";
  static const int MAX_BUFFER_SIZE = 128;

  static void commandsTaskEntry(void* param) {
    TCPCommands* self = static_cast<TCPCommands*>(param);
    self->tcpLoop();
  }

  void processCommand(String command) {

    //Serial.println("X Processing command: " + command);

    command.trim();
    if (command.length() == 0) return;

    int spaceIndex = command.indexOf(' ');
    String commandType = (spaceIndex > 0) ? command.substring(0, spaceIndex) : command;
    String value = (spaceIndex > 0) ? command.substring(spaceIndex + 1) : "";

    if (commandType == "H") {
      headingSetting->setValue(value.toInt());
      return;
    }

    if (commandType == "S") {
      speedSettings->setValue(value.toInt());
      return;
    }

    if (commandType == "A") {
      altitudeSettings->setValue(value.toInt());
      return;
    }

    if (commandType == "VS") {
      verticalSpeedSettings->setValue(value.toInt());
      return;
    }

    if (commandType == "AP") {
      if(value == "ON") {
        ioExpander->setLED(5, true);
      } else if(value == "OFF") {
        ioExpander->setLED(5, false);
      }
      return;
    }

    if(commandType == "AP_HEADING") {
      if(value == "ON") {
        headingSetting->headingOn();
      } else if(value == "OFF") {
        headingSetting->headingOff();
      }
      return;
    }

    if(commandType == "AP_LNAV") {
      if(value == "ON") {
        headingSetting->navOn();
      } else if(value == "OFF") {
        headingSetting->navOff();
      }
      return;
    }

    if(commandType == "AP_VNAV") {
      if(value == "ON") {
        verticalSpeedSettings->vnavOn();
      } else if(value == "OFF") {
        verticalSpeedSettings->vnavOff();
      }
      return;
    }

    if(commandType == "AP_SPEED") {
      if(value == "ON") {
        speedSettings->speedOn();
      } else if(value == "OFF") {
        speedSettings->speedOff();
      }
      return;
    }

    if(commandType == "AP_ALTITUDE") {
      if(value == "ON") {
        altitudeSettings->altitudeOn();
      } else if(value == "OFF") {
        altitudeSettings->altitudeOff();
      }
      return;
    }

    if(commandType == "AP_VS") {
      if(value == "ON") {
        verticalSpeedSettings->vsOn();
      } else if(value == "OFF") {
        verticalSpeedSettings->vsOff();
      }
      return;
    }
  }

  void tcpLoop() {
    // Wait for WiFi to be initialized and connected
    vTaskDelay(pdMS_TO_TICKS(2000));

    TCPSender* sender = TCPSender::getInstance();
    WiFiClient* client = sender->getClient();

    for (;;) {
      vTaskDelay(pdMS_TO_TICKS(10));

      if (sender->isConnected()) {
        while (client->available() > 0) {
          char c = client->read();

          if (c == '\n' || c == '\r') {
            if (buffer.length() > 0) {
              processCommand(buffer);
              buffer = "";
            }
          } else {
            buffer += c;
            if (buffer.length() >= MAX_BUFFER_SIZE) {
              buffer = "";
            }
          }
        }
      } else {
        // Clear buffer on disconnect
        buffer = "";
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }
  }

public:
  TCPCommands(HeadingSetting* headingSetting,
              SpeedSettings* speedSettings,
              AltitudeSettings* altitudeSettings,
              VerticalSpeedSettings* verticalSpeedSettings,
              IOExpander* ioExpander) {


    this->headingSetting = headingSetting;
    this->speedSettings = speedSettings;
    this->altitudeSettings = altitudeSettings;
    this->verticalSpeedSettings = verticalSpeedSettings;
    this->ioExpander = ioExpander;

    // Initialize WiFi
    Serial.println("X Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      vTaskDelay(pdMS_TO_TICKS(500));
      Serial.print("X .");
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.print("X WiFi connected! IP: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println();
      Serial.println("X WiFi connection failed!");
    }

    // Initialize TCP sender
    TCPSender::initialize(SERVER_IP, SERVER_PORT);

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
