#ifndef HAPTIC_KNOB_H
#define HAPTIC_KNOB_H

#include <SimpleFOC.h>
#include <MotorDriver.h>
#include "AutopilotSetting.h"

class HapticKnob {
private:
  MotorDriver* motorDriver;
  AutopilotSetting* autopilotSetting;
  float maxVoltage;
  float step;
  String name;
  float halfStep;
  inline static float targetAngle = 3;

public:

  HapticKnob(String name, MotorDriver* motorDriver, float maxVolt, float stepSize, AutopilotSetting* autopilotSetting) {
    this->motorDriver = motorDriver;
    this->autopilotSetting = autopilotSetting;
    this->name = name;
    this->maxVoltage = maxVolt;
    this->step = stepSize;
    this->halfStep = stepSize / 2;
  }

  void move() {
    motorDriver->loopFOC();

    float angle = motorDriver->shaftAngle();

    float tempMaxVoltage = maxVoltage;

    float angleDiff = abs(targetAngle - angle);

    if (angleDiff > halfStep) {
      if (targetAngle - angle < 0) {
        if (!autopilotSetting->isAtMinimum()) {
          targetAngle += step;
          this->autopilotSetting->minus();
        } else {
          tempMaxVoltage *= 3;
        }
      } else {
        if (!autopilotSetting->isAtMaximum()) {
          targetAngle -= step;
          this->autopilotSetting->plus();
        } else {
          tempMaxVoltage *= 3;
        }
      }
    }

    float distance = targetAngle - angle;

    if (distance > halfStep) {
      distance = halfStep;
    }
    if (distance < -halfStep) {
      distance = -halfStep;
    }

    float relativeDistance = distance / halfStep;

    if (relativeDistance < 0) {
      relativeDistance = relativeDistance * -relativeDistance;
    } else {
      relativeDistance = relativeDistance * relativeDistance;
    }

    float targetVoltage = tempMaxVoltage * relativeDistance;

    // Apply voltage (SimpleFOC will handle limiting via motor.voltage_limit)
    motorDriver->move(targetVoltage);
  }

  String getName() {
    return name;
  }
};

#endif
