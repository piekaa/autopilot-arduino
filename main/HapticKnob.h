#ifndef HAPTIC_KNOB_H
#define HAPTIC_KNOB_H

#include <SimpleFOC.h>

class HapticKnob {
private:
  BLDCMotor& motor;
  float halfStep;
  inline static float targetAngle = 3;  // inline static allows initialization here

public:
  float maxVoltage;
  float step;
  String name;

  // Constructor - implementation right here in the .h file!
  HapticKnob(String name, BLDCMotor& motorRef, float maxVolt, float stepSize)
    : motor(motorRef) {  // Initialize reference in initializer list
    this->name = name;
    this->maxVoltage = maxVolt;
    this->step = stepSize;
    this->halfStep = stepSize / 2;
  }

  // move() method - also right here!
  void move() {
    motor.loopFOC();

    float angle = motor.shaft_angle;

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
    motor.move(targetVoltage);
  }
};

#endif
