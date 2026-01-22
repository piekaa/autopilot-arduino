// #ifndef KNOB_MANAGER_H
// #define KNOB_MANAGER_H

// #include "HapticKnob.h"

// class KnobManager {
// private:
//   HapticKnob* knobs[4];  // Array of pointers to the 4 knobs
//   int currentIndex;

//   // Button pins
//   int buttonNextPin;
//   int buttonPrevPin;

//   // Debouncing
//   unsigned long lastButtonPressTime;
//   const unsigned long debounceDelay = 200; // 200ms debounce

// public:
//   // Constructor - takes references to all 4 knobs and button pins
//   KnobManager(HapticKnob& knob0, HapticKnob& knob1, HapticKnob& knob2, HapticKnob& knob3,
//               int nextPin = 13, int prevPin = 14)
//     : currentIndex(0), buttonNextPin(nextPin), buttonPrevPin(prevPin), lastButtonPressTime(0) {
//     // Store pointers to knobs
//     knobs[0] = &knob0;
//     knobs[1] = &knob1;
//     knobs[2] = &knob2;
//     knobs[3] = &knob3;

//     // Setup button pins
//     pinMode(buttonNextPin, INPUT_PULLUP);  // Use internal pull-up resistors
//     pinMode(buttonPrevPin, INPUT_PULLUP);
//   }

//   // Get the currently active knob
//   HapticKnob& getCurrentKnob() {
//     return *knobs[currentIndex];
//   }

//   // Switch to next knob (stops at last)
//   void next() {
//     if (currentIndex < 3) {  // Don't wrap around
//       currentIndex++;
//       Serial.println("KS");
//       Serial.println(knobs[currentIndex]->name);
//     }
//   }

//   // Switch to previous knob (stops at first)
//   void previous() {
//     if (currentIndex > 0) {  // Don't wrap around
//       currentIndex--;
//       Serial.println("KS");
//       Serial.println(knobs[currentIndex]->name);
//     }
//   }

//   // Check buttons and switch knobs if pressed
//   void update() {
//     unsigned long currentTime = millis();

//     // Check if enough time has passed since last button press (debouncing)
//     if (currentTime - lastButtonPressTime > debounceDelay) {

//       // Button pressed = LOW (because of INPUT_PULLUP)
//       if (digitalRead(buttonNextPin) == LOW) {
//         next();
//         lastButtonPressTime = currentTime;
//       }

//       if (digitalRead(buttonPrevPin) == LOW) {
//         previous();
//         lastButtonPressTime = currentTime;
//       }
//     }
//   }

//   // Get current index (useful for display)
//   int getCurrentIndex() {
//     return currentIndex;
//   }
// };

// #endif
