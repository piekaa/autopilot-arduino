#include <Wire.h>
#include <SimpleFOC.h>
#include <SparkFun_TMAG5273_Arduino_Library.h>
#include <WiFi.h>
#include "KnobManager.h"
#include "IC2Multiplexer.h"
#include "TCPCommands.h"
#include "IOExpander.h"

TCPCommands* tcpCommands;
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



  // Initialize IO Expander on multiplexer channel 4, I2C address 0x20
  ioExpander = new IOExpander(ic2Multiplexer, 4, 0x20);

  // Configure pin 8 (PB0) as button input with pullup
  ioExpander->configurePinAsInput(12, []() {
    TCPSender* sender = TCPSender::getInstance();
    if (sender) {
      sender->sendLine("AP_HEADING TOGGLE");
    }
  });

  ioExpander->configurePinAsInput(13, []() {
    TCPSender* sender = TCPSender::getInstance();
    if (sender) {
      sender->sendLine("AP_SPEED TOGGLE");
    }
  });

  ioExpander->configurePinAsInput(14, []() {
    TCPSender* sender = TCPSender::getInstance();
    if (sender) {
      sender->sendLine("AP_ALTITUDE TOGGLE");
    }
  });

  ioExpander->configurePinAsInput(15, []() {
    TCPSender* sender = TCPSender::getInstance();
    if (sender) {
      sender->sendLine("AP_VS TOGGLE");
    }
  });

  ioExpander->configurePinAsInput(0, []() {
    TCPSender* sender = TCPSender::getInstance();
    if (sender) {
      sender->sendLine("AP_VNAV TOGGLE");
    }
  });

  ioExpander->configurePinAsInput(1, []() {
    TCPSender* sender = TCPSender::getInstance();
    if (sender) {
      sender->sendLine("AP TOGGLE");
    }
  });

  ioExpander->configurePinAsInput(2, []() {
    TCPSender* sender = TCPSender::getInstance();
    if (sender) {
      sender->sendLine("AP_LNAV TOGGLE");
    }
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


  // displays = new AutopilotDisplays(ic2Multiplexer);
  tcpCommands = new TCPCommands(headingSetting, speedSettings, altitudeSettings, verticalSpeedSettings, ioExpander);

}


// ===== ARDUINO LOOP =====

void loop() {
  knobManager->update();
}
