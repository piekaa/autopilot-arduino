// Serial command handling with forwarding to custom serial port
// Uses HardwareSerial (UART2) for ESP32

// Custom serial port using UART2 on ESP32
// RX pin 15, TX pin 27
HardwareSerial customSerial(2);

void setupCustomSerial() {
  customSerial.begin(115200, SERIAL_8N1, 15, 27); // Initialize UART2 at 115200 baud
}

void handleSerialCommands() {
  if (Serial.available() > 0) {
    String commandType = "";

    // Read the command type
    char firstChar = Serial.read();
    commandType += firstChar;

    // Check if it's a two-character command (VS or SS)
    if (firstChar == 'V' || firstChar == 'S') {
      // Peek for potential second character
      delay(10); // Small delay to allow second character to arrive
      if (Serial.available() > 0) {
        char secondChar = Serial.peek();
        if (secondChar == 'S') {
          commandType += Serial.read(); // Add 'S' to make "VS" or "SS"
        }
      }
    }

    // Check if command type is supported (H, A, VS, S)
    if (commandType == "H" || commandType == "A" || commandType == "VS" || commandType == "S") {
      // Wait for the value on the next line
      while (Serial.available() == 0) {
        delay(1); // Wait for data
      }

      String value = Serial.readStringUntil('\n');
      value.trim(); // Remove any whitespace

      // Forward to custom serial port
      customSerial.print(commandType);
      customSerial.println(value);
    } else {
      // Flush remaining input
      while (Serial.available() > 0) {
        Serial.read();
      }
    }
  }
}
