#include <Wire.h>
#include <SimpleFOC.h>
#include <SparkFun_TMAG5273_Arduino_Library.h>
#include "KnobManager.h"
#include "IC2Multiplexer.h"
#include "AutopilotDisplays.h"
#include "SerialCommands.h"

AutopilotDisplays* displays;
SerialCommands* serialCommands;
KnobManager* knobManager;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  delay(100);  // Give I2C time to initialize

  knobManager = new KnobManager();

  // delay(1000);
  // // Initialize I2C with ESP32 pins (SDA=21, SCL=22)
  // Wire.begin(21, 22);

  // delay(100);  // Give I2C time to initialize
  // showOnDisplays();
  // Serial.println("Haptic Detent Knob Starting...");
  // // Initialize sensor
  // sensor.init();
  // // Initialize motor and driver
  // initMotorDriver();
  // delay(1000);
  // lastMillis = millis();
  // setupCustomSerial();


  IC2Multiplexer* ic2Multiplexer = new IC2Multiplexer();
  displays = new AutopilotDisplays(ic2Multiplexer);
  serialCommands = new SerialCommands(displays);
}


// ===== ARDUINO LOOP =====

void loop() {
  knobManager->update();
}
