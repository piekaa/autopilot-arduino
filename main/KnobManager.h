#ifndef KNOB_MANAGER_H
#define KNOB_MANAGER_H

#include "HapticKnob.h"
#include "MagneticSensor.h"
#include "AutopilotSetting.h"
#include "HeadingSetting.h"
#include "SpeedSettings.h"
#include "AltitudeSettings.h"
#include "VerticalSpeedSettings.h"
#include "IC2Multiplexer.h"
#include "Display.h"

class KnobManager {
private:
  HapticKnob* knobs[4];  // Array of pointers to the 4 knobs
  IC2Multiplexer* ic2Multiplexer;
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



public:
  KnobManager(IC2Multiplexer* ic2Multiplexer) {
    this->ic2Multiplexer = ic2Multiplexer;



    MagneticSensor* magneticSensor = new MagneticSensor();
    MotorDriver* motorDriver = new MotorDriver(magneticSensor);


    Display* headingDisplay = new Display(ic2Multiplexer, 0);
    Display* speedDisplay = new Display(ic2Multiplexer, 7);
    Display* altitudeDisplay = new Display(ic2Multiplexer, 6);
    Display* verticalSpeedDisplay = new Display(ic2Multiplexer, 5);


    HeadingSetting* headingSetting = new HeadingSetting(headingDisplay);
    SpeedSettings* speedSettings = new SpeedSettings(speedDisplay);
    AltitudeSettings* altitudeSettings = new AltitudeSettings(altitudeDisplay);
    VerticalSpeedSettings* verticalSpeedSettings = new VerticalSpeedSettings(verticalSpeedDisplay);

    knobs[0] = new HapticKnob("H", motorDriver, 0.3, 0.3, headingSetting);
    knobs[1] = new HapticKnob("S", motorDriver, 0.3, 0.3, speedSettings);
    knobs[2] = new HapticKnob("A", motorDriver, 0.3, 0.3, altitudeSettings);
    knobs[3] = new HapticKnob("VS", motorDriver, 1, 1, verticalSpeedSettings);




    // Setup button pins
    pinMode(buttonNextPin, INPUT_PULLUP);  // Use internal pull-up resistors
    pinMode(buttonPrevPin, INPUT_PULLUP);

    next();
  }

  void next() {
    if (currentIndex < 3) {
      currentIndex++;
    }
  }

  void previous() {
    if (currentIndex > 0) {
      currentIndex--;
    }
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
