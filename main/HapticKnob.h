#ifndef HAPTIC_KNOB_H
#define HAPTIC_KNOB_H

#include <SimpleFOC.h>
#include <MotorDriver.h>
#include <MotorDriver.h>

class HapticKnob {
private:
  MotorDriver* motorDriver;
  float maxVoltage;
  float step;
  String name;
  float halfStep;
  inline static float targetAngle = 3;  // inline static allows initialization here

public:
  
  HapticKnob(String name, MotorDriver* motorDriver, float maxVolt, float stepSize) {
    this->motorDriver = motorDriver;
    this->name = name;
    this->maxVoltage = maxVolt;
    this->step = stepSize;
    this->halfStep = stepSize / 2;
  }

  void move() {
    motorDriver->loopFOC();

    float angle = motorDriver->shaftAngle();

    if(abs(targetAngle - angle) > halfStep) {
      Serial.println(name);
      if(targetAngle - angle < 0) {
        targetAngle += step;
        Serial.println("-");
      } else {
        targetAngle -= step;
        Serial.println("+");
      }
    }

    float distance = targetAngle - angle;

    if(distance > halfStep) {
      distance = halfStep;
    }
    if(distance < -halfStep) {
      distance = -halfStep;
    }

    float relativeDistance = distance / halfStep;

    if(relativeDistance < 0) {
      relativeDistance = relativeDistance * -relativeDistance;
    } else {
      relativeDistance = relativeDistance * relativeDistance;
    }

    float targetVoltage = maxVoltage * relativeDistance;

    // Apply voltage (SimpleFOC will handle limiting via motor.voltage_limit)
    motorDriver->move(targetVoltage);
  }

  String getName() {
    return name;
  }
};

#endif
