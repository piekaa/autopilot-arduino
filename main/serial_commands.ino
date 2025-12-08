// Serial command handling
// Note: knob is declared in main.ino
void handleSerialCommands() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == 'V' || cmd == 'v') {
      float value = Serial.parseFloat();
      knob.maxVoltage = value;
      Serial.print("maxVoltage set to: ");
      Serial.println(knob.maxVoltage);
    }

    if (cmd == 'S' || cmd == 's') {
      float value = Serial.parseFloat();
      knob.step = value;
      Serial.print("step set to: ");
      Serial.println(knob.step);
    }
    // Clear any remaining characters in buffer
    while(Serial.available()) Serial.read();
  }
}