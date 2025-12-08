// Motor and driver functions and configuration
// Note: motor, driver, sensor, and DRIVER_ENABLE_PIN are declared in main.ino

// Initialize motor driver and configure motor
void initMotorDriver() {
  // Enable TMC6300 driver
  pinMode(DRIVER_ENABLE_PIN, OUTPUT);
  digitalWrite(DRIVER_ENABLE_PIN, HIGH);

  // ===== DRIVER CONFIGURATION =====
  driver.voltage_power_supply = 3.3;
  driver.voltage_limit = 2;
  driver.pwm_frequency = 20000;  // Lower PWM to reduce EMI on I2C
  driver.init();

  // Link driver to motor
  motor.linkDriver(&driver);

  // ===== MOTOR CONFIGURATION =====
  motor.voltage_limit = 3;

  // Use voltage/torque control mode for direct haptic control
  motor.controller = MotionControlType::torque;
  motor.torque_controller = TorqueControlType::voltage;

  // Velocity low-pass filter for damping calculation
  motor.LPF_velocity.Tf = 0.01;  // Light filtering for velocity estimation

  // Link sensor to motor
  motor.linkSensor(&sensor);

  // ===== INITIALIZE FOC =====
  motor.init();
  motor.initFOC();
}
