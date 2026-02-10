#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IC2Multiplexer.h>

class Display {
  IC2Multiplexer* ic2Multiplexer;
  uint8_t channel;
  Adafruit_SSD1306* oledDisplay;
  SemaphoreHandle_t i2cMutex;
public:
  Display(IC2Multiplexer* ic2Multiplexer, uint8_t channel, SemaphoreHandle_t i2cMutex) {
    this->ic2Multiplexer = ic2Multiplexer;
    this->channel = channel;
    this->i2cMutex = i2cMutex;
    this->oledDisplay = new Adafruit_SSD1306(128, 64, &Wire1, -1);

    // Check if mutex is valid
    if(i2cMutex != NULL && xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(2000)) == pdTRUE) {
      ic2Multiplexer->selectChannel(channel);
      bool displayInitialized = oledDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C);
      if (displayInitialized) {
        oledDisplay->clearDisplay();
        oledDisplay->println("display ready");
        oledDisplay->display();
      } else {
        Serial.println("X Failed to initialize display");
      }
      xSemaphoreGive(i2cMutex);
    } else {
      if(i2cMutex == NULL) {
        Serial.println("X I2C mutex is NULL in Display constructor");
      } else {
        Serial.println("X Failed to take I2C mutex in Display constructor");
      }
      // Initialize without mutex protection as fallback
      ic2Multiplexer->selectChannel(channel);
      oledDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    }
  }

  void showText(String text) {
    // Check if mutex is valid
    if(i2cMutex != NULL && xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
      ic2Multiplexer->selectChannel(channel);
      oledDisplay->clearDisplay();
      oledDisplay->setTextSize(2);
      oledDisplay->setTextColor(SSD1306_WHITE);
      oledDisplay->setCursor(0, 0);
      oledDisplay->println(text);
      oledDisplay->display();
      xSemaphoreGive(i2cMutex);
    } else {
      if(i2cMutex == NULL) {
        Serial.println("X I2C mutex is NULL in showText");
      } else {
        Serial.println("X Failed to take I2C mutex in showText");
      }
      // Proceed without mutex as fallback (risky but prevents crash)
      ic2Multiplexer->selectChannel(channel);
      oledDisplay->clearDisplay();
      oledDisplay->setTextSize(2);
      oledDisplay->setTextColor(SSD1306_WHITE);
      oledDisplay->setCursor(0, 0);
      oledDisplay->println(text);
      oledDisplay->display();
    }
  }
};

#endif