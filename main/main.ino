#include <Wire.h>
#include <SimpleFOC.h>
#include <SparkFun_TMAG5273_Arduino_Library.h>
#include "KnobManager.h"
#include "IC2Multiplexer.h"
#include "AutopilotDisplays.h"
#include "SerialCommands.h"
#include "IOExpander.h"

AutopilotDisplays* displays;
SerialCommands* serialCommands;
KnobManager* knobManager;
IOExpander* ioExpander;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  knobManager = new KnobManager();

  IC2Multiplexer* ic2Multiplexer = new IC2Multiplexer();
  displays = new AutopilotDisplays(ic2Multiplexer);
  serialCommands = new SerialCommands(displays);

  // Initialize IO Expander on multiplexer channel 4, I2C address 0x20
  ioExpander = new IOExpander(ic2Multiplexer, 4, 0x20);

  // Configure pin 8 (PB0) as button input with pullup
  ioExpander->configurePinAsInput(8, true);

  // Set callback for button press
  ioExpander->setButtonCallback(8, []() {
    Serial.println("Button PB0 pressed!");
  });
}


// ===== ARDUINO LOOP =====

void loop() {
  knobManager->update();
}
