#ifndef KNOB_MANAGER_H
#define KNOB_MANAGER_H

#include "HapticKnob.h"
#include "MagneticSensor.h"

class KnobManager {
private:
  HapticKnob* knobs[4];  // Array of pointers to the 4 knobs
  int currentIndex = 0;

  // Button pins
  int buttonNextPin = 13;
  int buttonPrevPin = 14;

  // Debouncing
  unsigned long lastButtonPressTime = 0;
  const unsigned long debounceDelay = 200;  // 200ms debounce

  // Get the currently active knob
  HapticKnob* getCurrentKnob() {
    return knobs[currentIndex];
  }

  // Switch to next knob (stops at last)
  void next() {
    if (currentIndex < 3) {  // Don't wrap around
      currentIndex++;
      Serial.println("KS");
      Serial.println(knobs[currentIndex]->getName());
    }
  }

  // Switch to previous knob (stops at first)
  void previous() {
    if (currentIndex > 0) {  // Don't wrap around
      currentIndex--;
      Serial.println("KS");
      Serial.println(knobs[currentIndex]->getName());
    }
  }

public:
  KnobManager() {
    MagneticSensor* magneticSensor = new MagneticSensor();
    MotorDriver* motorDriver = new MotorDriver(magneticSensor);

    HapticKnob knobVerticalSpeed = HapticKnob("VS", motorDriver, 1, 1);
    HapticKnob knobHeading = HapticKnob("H", motorDriver, 0.3, 0.3);
    HapticKnob knobAltitude = HapticKnob("A", motorDriver, 0.3, 0.3);
    HapticKnob knobSpeed = HapticKnob("S", motorDriver, 0.3, 0.3);

    // Setup button pins
    pinMode(buttonNextPin, INPUT_PULLUP);  // Use internal pull-up resistors
    pinMode(buttonPrevPin, INPUT_PULLUP);
  }

  // Check buttons and switch knobs if pressed
  void update() {
    unsigned long currentTime = millis();

    getCurrentKnob()->move();

    // Check if enough time has passed since last button press (debouncing)
    if (currentTime - lastButtonPressTime > debounceDelay) {

      // Button pressed = LOW (because of INPUT_PULLUP)
      if (digitalRead(buttonNextPin) == LOW) {
        next();
        lastButtonPressTime = currentTime;
      }

      if (digitalRead(buttonPrevPin) == LOW) {
        previous();
        lastButtonPressTime = currentTime;
      }
    }
  }
};

#endif
