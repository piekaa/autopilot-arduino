#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "MagneticSensor.h"
#include <SimpleFOC.h>

class MotorDriver {

  BLDCMotor motor = BLDCMotor(7);
  BLDCDriver6PWM driver = BLDCDriver6PWM(16, 17, 18, 23, 19, 33);
  GenericSensor* sensorWrapper;

  inline static MagneticSensor* magneticSensor; 

  static float readSensorAngle() {
    return magneticSensor->readAngle();
  }

  static void initSensor() {
    magneticSensor->init();
  }

public:
  MotorDriver(MagneticSensor* magneticSensor) {
    this->magneticSensor = magneticSensor;

    sensorWrapper = new GenericSensor(readSensorAngle, initSensor);

    // Explicitly initialize the sensor BEFORE motor init
    sensorWrapper->init();

    int driverEnablePin = 5;
    pinMode(driverEnablePin, OUTPUT);
    digitalWrite(driverEnablePin, HIGH);

    // ===== DRIVER CONFIGURATION =====
    driver.voltage_power_supply = 3.3;
    driver.voltage_limit = 2;
    driver.pwm_frequency = 20000;  // Lower PWM to reduce EMI on I2C
    driver.init();

    // Link driver to motor
    motor.linkDriver(&driver);

    // ===== MOTOR CONFIGURATION =====
    motor.voltage_limit = 3;

    // Use voltage/torque control mode for direct haptic control
    motor.controller = MotionControlType::torque;
    motor.torque_controller = TorqueControlType::voltage;

    // Velocity low-pass filter for damping calculation
    motor.LPF_velocity.Tf = 0.01;  // Light filtering for velocity estimation

    // Link sensor to motor
    motor.linkSensor(sensorWrapper);

    // ===== INITIALIZE FOC =====
    motor.init();
    motor.initFOC();

    delay(1000);  // Delay after motor init like in working version
  }

  void loopFOC() {
    sensorWrapper->update();
    motor.loopFOC();
  }

  void move(float targetVoltage) {
    motor.move(targetVoltage);
  }

  float shaftAngle() {
    return motor.shaft_angle;
  }
};

#endif