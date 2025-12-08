#ifndef HAPTIC_KNOB_H
#define HAPTIC_KNOB_H

#include <SimpleFOC.h>

class HapticKnob {
private:
  BLDCMotor& motor;
  float halfStep;
  String name;
  float targetAngle = 3;

public:
  float maxVoltage;
  float step;
  int value;

  // Constructor - implementation right here in the .h file!
  HapticKnob(String name, BLDCMotor& motorRef, float maxVolt, float stepSize, int val)
    : motor(motorRef) {  // Initialize reference in initializer list
    this->name = name;
    this->maxVoltage = maxVolt;
    this->step = stepSize;
    this->halfStep = stepSize / 2;
    this->value = val;
  }

  // move() method - also right here!
  void move() {
    float angle = motor.shaft_angle;

    if(abs(targetAngle - angle) > halfStep) {
      if(targetAngle - angle < 0) {
        targetAngle += step;
        value--;
      } else {
        targetAngle -= step;
        value++;
      }
      Serial.println(name);
      Serial.println(value);
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
