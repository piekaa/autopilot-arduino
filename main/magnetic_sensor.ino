// // Magnetic sensor functions and configuration
// // Note: magneticSensor, angleSamples, and sensor are declared in main.ino

// // Initialize the TMAG5273 magnetic sensor
// void initSensor() {
//   // NOTE: Your board has the sensor at 0x22, not the documented 0x35!
//   if (!magneticSensor.begin(0x22, Wire)) {
//     Serial.println("TMAG5273 sensor not found at 0x22!");
//     Serial.println("I2C Scanner found it at this address, but library can't initialize it.");
//     Serial.println("Check if library version is compatible.");
//     // while(1);
//   }

//   Serial.println("TMAG5273 sensor found at 0x22!");

//   // Configure sensor for angle measurement (XYX mode for rotation about Z-axis)
//   magneticSensor.setMagneticChannel(TMAG5273_XYX_ENABLE);
//   magneticSensor.setAngleEn(TMAG5273_XY_ANGLE_CALCULATION);
//   magneticSensor.setConvAvg(TMAG5273_X4_CONVERSION);
// }

// // Function to read current angle from sensor (in radians)
// float readSensorAngle() {
//   if (magneticSensor.getMagneticChannel() != 0) {

//     int numberOfReadings = 5;
//     float readings[numberOfReadings];

//     // Collect readings
//     for(int i = 0 ; i < numberOfReadings ; i++) {
//       readings[i] = magneticSensor.getAngleResult();
//     }

//     // Sort readings for median calculation
//     for(int i = 0; i < numberOfReadings - 1; i++) {
//       for(int j = i + 1; j < numberOfReadings; j++) {
//         if(readings[i] > readings[j]) {
//           float temp = readings[i];
//           readings[i] = readings[j];
//           readings[j] = temp;
//         }
//       }
//     }

//     // Calculate median (average of two middle values for even number of readings)
//     float angle_degrees = readings[numberOfReadings/2];
//     float angle_rad = angle_degrees * PI / 180.0;

//     return angle_rad;
//   }
//   return magneticSensor.getAngleResult();
// }
