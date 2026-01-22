#ifndef SERIAL_COMMANDS_H
#define SERIAL_COMMANDS_H

class SerialCommands {

  AutopilotDisplays* displays;

  static void commandsTaskEntry(void* param) {
    SerialCommands* self = static_cast<SerialCommands*>(param);
    self->serialLoop();
  }

  void serialLoop() {
    for (;;) {
      vTaskDelay(pdMS_TO_TICKS(100));
      if (Serial.available() > 0) {
        char commandType = Serial.read();

        if (commandType == 'D') {
          int heading = Serial.parseInt();
          int speed = Serial.parseInt();
          int altitude = Serial.parseInt();
          int verticalSpeed = Serial.parseInt();
          displays->showValues(heading, speed, altitude, verticalSpeed);
        } else {
          Serial.println("X Unknown command type: " + commandType);
        }
        Serial.readStringUntil('\n');
      }
    }
  }

public:
  SerialCommands(AutopilotDisplays* displays) {
    this->displays = displays;
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
