#include <Wire.h>
#include <SimpleFOC.h>
#include <SparkFun_TMAG5273_Arduino_Library.h>
#include "KnobManager.h"
#include "IC2Multiplexer.h"
#include "SerialCommands.h"
#include "IOExpander.h"

SerialCommands* serialCommands;
KnobManager* knobManager;
IOExpander* ioExpander;

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);

  IC2Multiplexer* ic2Multiplexer = new IC2Multiplexer();


  Display* headingDisplay = new Display(ic2Multiplexer, 0);
  Display* speedDisplay = new Display(ic2Multiplexer, 7);
  Display* altitudeDisplay = new Display(ic2Multiplexer, 6);
  Display* verticalSpeedDisplay = new Display(ic2Multiplexer, 5);


  HeadingSetting* headingSetting = new HeadingSetting(headingDisplay);
  SpeedSettings* speedSettings = new SpeedSettings(speedDisplay);
  AltitudeSettings* altitudeSettings = new AltitudeSettings(altitudeDisplay);
  VerticalSpeedSettings* verticalSpeedSettings = new VerticalSpeedSettings(verticalSpeedDisplay);


  knobManager = new KnobManager(ic2Multiplexer, headingSetting, speedSettings, altitudeSettings, verticalSpeedSettings);


  // displays = new AutopilotDisplays(ic2Multiplexer);
  serialCommands = new SerialCommands(headingSetting, speedSettings, altitudeSettings, verticalSpeedSettings);

  // Initialize IO Expander on multiplexer channel 4, I2C address 0x20
  ioExpander = new IOExpander(ic2Multiplexer, 4, 0x20);

  // Configure pin 8 (PB0) as button input with pullup
  ioExpander->configurePinAsInput(12, []() {
    Serial.println("X Button PB4 pressed!");
  });

  ioExpander->configurePinAsInput(13, []() {
    Serial.println("X Button PB5 pressed!");
  });

  ioExpander->configurePinAsInput(14, []() {
    Serial.println("X Button PB6 pressed!");
  });

  ioExpander->configurePinAsInput(15, []() {
    Serial.println("X Button PB7 pressed!");
  });

  ioExpander->configurePinAsInput(0, []() {
    Serial.println("X Button PA0 pressed!");
  });

  ioExpander->configurePinAsInput(1, []() {
    Serial.println("X Button PA1 pressed!");
  });

  ioExpander->configurePinAsInput(2, []() {
    Serial.println("X Button PA2 pressed!");
  });

  ioExpander->configurePinAsInput(3, [knobManager]() {
    Serial.println("X Button PA3 pressed!");
    knobManager->next();
  });

  ioExpander->configurePinAsInput(4, [knobManager]() {
    Serial.println("X Button PA4 pressed!");
    knobManager->previous();
  });

  ioExpander->configurePinAsOutput(5);
  ioExpander->setLED(5, true);
}


// ===== ARDUINO LOOP =====

void loop() {
  knobManager->update();
}
