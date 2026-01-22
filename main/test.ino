// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// #include <Wire.h>

// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define OLED_RESET -1
// #define TCAADDR 0x70

// // Custom I2C pins for display
// #define SDA_PIN 15
// #define SCL_PIN 27

// // Use the existing Wire1 instance (already defined in ESP32 Wire library)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);

// // Task handle for display task
// TaskHandle_t DisplayTask;


// // Display-related functions and configuration
// // Note: display, TCAADDR, and display dimensions are declared in main.ino

// // TCA9548A I2C multiplexer channel selection
// void tcaSelect(uint8_t channel) {
//   Wire1.beginTransmission(TCAADDR);
//   Wire1.write(1 << channel);
//   Wire1.endTransmission();
// }

// // Initialize and display content on all connected displays
// void showOnDisplays() {
//   // Function to initialize and display text on a specific channel
//   auto initializeAndDisplay = [](uint8_t channel, int textSize, const char* text) {
//     tcaSelect(channel);

//     // Initialize the display
//     if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Assuming address 0x3C
//       // Handle initialization error if needed
//       return;
//     }

//     display.clearDisplay();
//     display.setTextSize(textSize);  // Set text size (1 is default)
//     display.setTextColor(SSD1306_WHITE);
//     display.setCursor(0, 0); // Set starting position

//     // Note: Adafruit library uses print()
//     display.println(text);
//     display.display(); // Show the content
//   };

//   // Channel 7
//   initializeAndDisplay(7, 2, "Piekoszek Podlaski");

//   // Channel 6
//   initializeAndDisplay(6, 4, "Kup Pan");

//   // Channel 5
//   initializeAndDisplay(5, 3, "Swinkie Morskie");
// }

// // Task function for display updates - runs on Core 0
// void DisplayTaskCode(void * pvParameters) {
//   // Initialize custom I2C bus with GPIO 15 (SDA) and GPIO 12 (SCL)
//   Wire1.begin(SDA_PIN, SCL_PIN);

//   for(;;) {
//     // Continuously update displays
//     showOnDisplays();
//     delay(1000);  // Update every second
//   }
// }



// void setup() {
//   Serial.begin(115200);
//   // Create task for display updates on Core 0
  // xTaskCreatePinnedToCore(
  //   DisplayTaskCode,    // Task function
  //   "DisplayTask",      // Task name
  //   10000,              // Stack size in bytes
  //   NULL,               // Task parameter
  //   1,                  // Priority
  //   &DisplayTask,       // Task handle
  //   0                   // Core 0
  // );
// }

// void loop() {
//   // Main loop runs on Core 1
//   // Display updates are handled on Core 0 by DisplayTask
//   // Add your other code here
//   Serial.println("Dupa");
//   delay(100);
// }
