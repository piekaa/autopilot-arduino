#ifndef IC2MULTIPLEXER_H
#define IC2MULTIPLEXER_H

class IC2Multiplexer {
private:
  SemaphoreHandle_t i2cMutex;

public:
  IC2Multiplexer() {
    // Create the I2C mutex
    i2cMutex = xSemaphoreCreateMutex();
    if (i2cMutex == NULL) {
      Serial.println("FATAL: Failed to create I2C mutex in I2CMultiplexer!");
    } else {
      Serial.println("I2C mutex created successfully in I2CMultiplexer");
    }

    Wire1.begin(15, 27); // SDA, SCL
  }

  void selectAndLockChannel(uint8_t channel) {
    // Attempt to acquire the mutex with 1000ms timeout
    if (i2cMutex != NULL && xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
      // Mutex acquired successfully, select the channel
      Wire1.beginTransmission(0x70);
      Wire1.write(1 << channel);
      Wire1.endTransmission();
    } else {
      // Mutex is NULL or timeout occurred, proceed anyway (fallback behavior)
      if (i2cMutex == NULL) {
        Serial.println("X I2C mutex is NULL in selectAndLockChannel");
      } else {
        Serial.println("X Failed to acquire I2C mutex in selectAndLockChannel");
      }
      // Select channel anyway
      Wire1.beginTransmission(0x70);
      Wire1.write(1 << channel);
      Wire1.endTransmission();
    }
  }

  void unlockChannel() {
    // Release the mutex if it was successfully acquired
    if (i2cMutex != NULL) {
      xSemaphoreGive(i2cMutex);
    }
  }
};

#endif