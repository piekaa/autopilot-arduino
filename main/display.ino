// Display-related functions and configuration
// Note: display, TCAADDR, and display dimensions are declared in main.ino

// TCA9548A I2C multiplexer channel selection
void tcaSelect(uint8_t channel) {
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// Initialize and display content on all connected displays
void showOnDisplays() {
  // Function to initialize and display text on a specific channel
  auto initializeAndDisplay = [](uint8_t channel, int textSize, const char* text) {
    tcaSelect(channel);

    // Initialize the display
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Assuming address 0x3C
      // Handle initialization error if needed
      return;
    }

    display.clearDisplay();
    display.setTextSize(textSize);  // Set text size (1 is default)
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0); // Set starting position

    // Note: Adafruit library uses print()
    display.println(text);
    display.display(); // Show the content
  };

  // Channel 7
  initializeAndDisplay(7, 2, "Piekoszek Podlaski");

  // Channel 6
  initializeAndDisplay(6, 4, "Kup Pan");

  // Channel 5
  initializeAndDisplay(5, 3, "Swinkie Morskie");
}
