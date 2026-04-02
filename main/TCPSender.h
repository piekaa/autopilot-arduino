#ifndef TCP_SENDER_H
#define TCP_SENDER_H

#include <WiFi.h>

class TCPSender {
private:
  static TCPSender* instance;
  WiFiClient client;
  const char* serverIP;
  uint16_t serverPort;
  SemaphoreHandle_t mutex;
  bool connected;

  TCPSender(const char* ip, uint16_t port) {
    serverIP = ip;
    serverPort = port;
    connected = false;
    mutex = xSemaphoreCreateMutex();
  }

  bool ensureConnected() {
    if (client.connected()) {
      return true;
    }

    Serial.println("X TCP: Attempting to connect to server...");
    connected = client.connect(serverIP, serverPort);

    if (connected) {
      Serial.println("X TCP: Connected to server");
    } else {
      Serial.println("X TCP: Connection failed");
    }

    return connected;
  }

public:
  static void initialize(const char* ip, uint16_t port) {
    if (instance == nullptr) {
      instance = new TCPSender(ip, port);
    }
  }

  static TCPSender* getInstance() {
    return instance;
  }

  void sendLine(const String& message) {
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      if (ensureConnected()) {
        client.print(message);
        client.print("\n");
        Serial.print("X TCP TX: ");
        Serial.println(message);
      }
      xSemaphoreGive(mutex);
    }
  }

  WiFiClient* getClient() {
    return &client;
  }

  bool isConnected() {
    return client.connected();
  }
};

// Initialize static member
TCPSender* TCPSender::instance = nullptr;

#endif
