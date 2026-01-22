#include <Wire.h>
#include <SimpleFOC.h>
#include <SparkFun_TMAG5273_Arduino_Library.h>
#include "HapticKnob.h"
#include "KnobManager.h"
#include "IC2Multiplexer.h"
#include "AutopilotDisplays.h"
#include "SerialCommands.h"
#include "MagneticSensor.h"
#include <MotorDriver.h>

AutopilotDisplays* displays;
SerialCommands* serialCommands;
MagneticSensor* magneticSensor;
MotorDriver* motorDriver;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);


  magneticSensor = new MagneticSensor();
  motorDriver = new MotorDriver(magneticSensor);

  HapticKnob knobVerticalSpeed = HapticKnob("VS", motorDriver, 1, 1);
  HapticKnob knobHeading = HapticKnob("H", motorDriver, 0.3, 0.3);
  HapticKnob knobAltitude = HapticKnob("A", motorDriver, 0.3, 0.3);
  HapticKnob knobSpeed = HapticKnob("S", motorDriver, 0.3, 0.3);

  KnobManager knobManager = KnobManager(knobVerticalSpeed, knobHeading, knobAltitude, knobSpeed, 13, 14);

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

  // ic2Multiplexer and displays are now initialized as global variables above


  IC2Multiplexer* ic2Multiplexer = new IC2Multiplexer();
  displays = new AutopilotDisplays(ic2Multiplexer);

  serialCommands = new SerialCommands(displays);
}


// ===== ARDUINO LOOP =====

void loop() {
}
