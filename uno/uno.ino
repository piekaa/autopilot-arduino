#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void tcaSelect(uint8_t channel) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << channel);
  Wire.endTransmission();
}


void initDisplay(uint8_t channel) {
  tcaSelect(channel);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // Display init failed
    return;
  }
  display.clearDisplay();
  display.display();
}

void show(String header, int value, uint8_t channel) {
  tcaSelect(channel);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.setTextSize(2);
  display.println(header);

  display.setCursor(10, 30);
  display.setTextSize(4);
  display.println(value);

  display.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();


  initDisplay(7);
  initDisplay(6);
  initDisplay(5);
  initDisplay(3);

  show("Heading", 0, 7);
  show("Altitude", 0, 6);
  show("VS", 0, 5);
  show("Speed", 0, 3);
}

void loop() {

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // Remove any whitespace

    if (command.length() > 0) {
      // Extract command type (first character)
      char type = command.charAt(0);

      // Extract value (rest of the string)
      int value = command.substring(1).toInt();

      // Process based on command type
      switch (type) {
        case 'H':
          show("Heading", value, 7);
          break;
        case 'V':
          show("VS", value, 5);
          break;
        case 'A':
          show("Altitude", value, 6);
          break;
        case 'S':
          show("Speed", value, 3);
          break;
        default:
          Serial.print("Unknown command: ");
          Serial.println(command);
          break;
      }
    }
  }
}
