// Serial command handling
// Note: knobManager is declared in main.ino
void handleSerialCommands() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == 'V' || cmd == 'v') {
      float value = Serial.parseFloat();
      knobManager.getCurrentKnob().maxVoltage = value;
      Serial.print("maxVoltage set to: ");
      Serial.println(knobManager.getCurrentKnob().maxVoltage);
    }

    if (cmd == 'S' || cmd == 's') {
      float value = Serial.parseFloat();
      knobManager.getCurrentKnob().step = value;
      Serial.print("step set to: ");
      Serial.println(knobManager.getCurrentKnob().step);
    }
    // Clear any remaining characters in buffer
    while(Serial.available()) Serial.read();
  }
}