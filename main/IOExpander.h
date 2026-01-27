#ifndef IO_EXPANDER_H
#define IO_EXPANDER_H

#include "IC2Multiplexer.h"
#include <functional>

// MCP23017 Register Addresses
#define MCP23017_IODIRA 0x00
#define MCP23017_IODIRB 0x01
#define MCP23017_GPPUA 0x0C
#define MCP23017_GPPUB 0x0D
#define MCP23017_GPIOA 0x12
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATA 0x14
#define MCP23017_OLATB 0x15

class IOExpander {
private:
  IC2Multiplexer* ic2Multiplexer;
  uint8_t channel;
  uint8_t address;

  // Pin configuration (1 = input, 0 = output)
  uint16_t inputMask = 0x0000;

  // Button state tracking
  uint16_t lastButtonState = 0xFFFF;
  unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;

  // LED state cache
  uint16_t ledState = 0x0000;

  // Per-pin callbacks (supports lambdas) - fired on button press only
  std::function<void()> pinCallbacks[16];

  static void taskEntry(void* param) {
    IOExpander* self = static_cast<IOExpander*>(param);
    self->taskLoop();
  }

  void taskLoop() {
    for(;;) {
      vTaskDelay(pdMS_TO_TICKS(10));
      updateInputs();
    }
  }

  void writeRegister(uint8_t reg, uint8_t value) {
    ic2Multiplexer->selectChannel(channel);
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    Wire1.write(value);
    Wire1.endTransmission();
  }

  uint8_t readRegister(uint8_t reg) {
    ic2Multiplexer->selectChannel(channel);
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    Wire1.endTransmission();

    Wire1.requestFrom(address, (uint8_t)1);
    if (Wire1.available()) {
      return Wire1.read();
    }
    return 0;
  }

  uint16_t readAllInputs() {
    uint8_t portA = readRegister(MCP23017_GPIOA);
    uint8_t portB = readRegister(MCP23017_GPIOB);
    return ((uint16_t)portB << 8) | portA;
  }

  void updateInputs() {
    if (inputMask == 0) return;

    uint16_t currentState = readAllInputs();
    unsigned long currentTime = millis();

    if (currentTime - lastDebounceTime > debounceDelay) {
      uint16_t changed = currentState ^ lastButtonState;

      if (changed != 0) {
        for (uint8_t pin = 0; pin < 16; pin++) {
          uint16_t pinMask = 1 << pin;

          if ((inputMask & pinMask) && (changed & pinMask)) {
            bool isPressed = !(currentState & pinMask);

            if (isPressed && pinCallbacks[pin]) {
              pinCallbacks[pin]();
            }
          }
        }

        lastButtonState = currentState;
        lastDebounceTime = currentTime;
      }
    }
  }

public:
  IOExpander(IC2Multiplexer* ic2Multiplexer, uint8_t channel, uint8_t address = 0x20) {
    this->ic2Multiplexer = ic2Multiplexer;
    this->channel = channel;
    this->address = address;

    writeRegister(MCP23017_IODIRA, 0xFF);
    writeRegister(MCP23017_IODIRB, 0xFF);

    writeRegister(MCP23017_GPPUA, 0x00);
    writeRegister(MCP23017_GPPUB, 0x00);

    writeRegister(MCP23017_OLATA, 0x00);
    writeRegister(MCP23017_OLATB, 0x00);

    Serial.println("IOExpander initialized");

    lastButtonState = readAllInputs();

    xTaskCreatePinnedToCore(
      taskEntry,
      "IOExpanderTask",
      4096,
      this,
      2,
      NULL,
      0
    );

    Serial.println("IOExpander task started on Core 0");
  }

  void configurePinAsInput(uint8_t pin, bool enablePullup = true) {
    if (pin > 15) return;

    uint16_t pinMask = 1 << pin;
    inputMask |= pinMask;

    if (pin < 8) {
      uint8_t currentDir = readRegister(MCP23017_IODIRA);
      writeRegister(MCP23017_IODIRA, currentDir | (1 << pin));

      if (enablePullup) {
        uint8_t currentPullup = readRegister(MCP23017_GPPUA);
        writeRegister(MCP23017_GPPUA, currentPullup | (1 << pin));
      }
    } else {
      uint8_t currentDir = readRegister(MCP23017_IODIRB);
      writeRegister(MCP23017_IODIRB, currentDir | (1 << (pin - 8)));

      if (enablePullup) {
        uint8_t currentPullup = readRegister(MCP23017_GPPUB);
        writeRegister(MCP23017_GPPUB, currentPullup | (1 << (pin - 8)));
      }
    }
  }

  void configurePinAsOutput(uint8_t pin) {
    if (pin > 15) return;

    uint16_t pinMask = 1 << pin;
    inputMask &= ~pinMask;

    if (pin < 8) {
      uint8_t currentDir = readRegister(MCP23017_IODIRA);
      writeRegister(MCP23017_IODIRA, currentDir & ~(1 << pin));
    } else {
      uint8_t currentDir = readRegister(MCP23017_IODIRB);
      writeRegister(MCP23017_IODIRB, currentDir & ~(1 << (pin - 8)));
    }
  }

  void setLED(uint8_t pin, bool state) {
    if (pin > 15) return;

    uint16_t pinMask = 1 << pin;

    if (state) {
      ledState |= pinMask;
    } else {
      ledState &= ~pinMask;
    }

    if (pin < 8) {
      writeRegister(MCP23017_OLATA, (uint8_t)(ledState & 0xFF));
    } else {
      writeRegister(MCP23017_OLATB, (uint8_t)((ledState >> 8) & 0xFF));
    }
  }

  void toggleLED(uint8_t pin) {
    if (pin > 15) return;

    uint16_t pinMask = 1 << pin;
    bool currentState = ledState & pinMask;
    setLED(pin, !currentState);
  }

  bool readButton(uint8_t pin) {
    if (pin > 15) return false;

    uint16_t currentState = readAllInputs();
    return !(currentState & (1 << pin));
  }

  void setButtonCallback(uint8_t pin, std::function<void()> callback) {
    if (pin > 15) return;
    pinCallbacks[pin] = callback;
  }
};

#endif
