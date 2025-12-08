#include <Wire.h>
#include <SimpleFOC.h>
#include <SparkFun_TMAG5273_Arduino_Library.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HapticKnob.h"

BLDCMotor motor = BLDCMotor(7);
BLDCDriver6PWM driver = BLDCDriver6PWM(16, 17, 18, 23, 19, 33);

// Magnetic sensor object
TMAG5273 magneticSensor;

// Display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define TCAADDR 0x70
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Forward declarations of functions defined in other .ino files
float readSensorAngle();
void initSensor();

// Generic sensor wrapper for SimpleFOC (uses functions from magnetic_sensor.ino)
GenericSensor sensor = GenericSensor(readSensorAngle, initSensor);

// Motor driver configuration
#define DRIVER_ENABLE_PIN 5

// ===== ARDUINO SETUP =====

long lastMillis;

HapticKnob knobVerticalSpeed = HapticKnob("VS", motor, 1, 1, 100);
HapticKnob knobHeading = HapticKnob("H", motor, 0.3, 0.3, 50);
HapticKnob knobAltitude = HapticKnob("A", motor, 0.3, 0.3, 50);
HapticKnob knobSpeed = HapticKnob("S", motor, 0.3, 0.3, 50);

void setup() {
  Serial.begin(115200);
  delay(1000);
  // Initialize I2C with ESP32 pins (SDA=21, SCL=22)
  Wire.begin(21, 22);
  delay(100);  // Give I2C time to initialize
  showOnDisplays();
  Serial.println("Haptic Detent Knob Starting...");
  // Initialize sensor
  sensor.init();
  // Initialize motor and driver
  initMotorDriver();
  delay(1000);
  lastMillis = millis();
}


// ===== ARDUINO LOOP =====

void loop() {
  motor.loopFOC();

  // Handle serial commands
  handleSerialCommands();

  knob.move();

}
