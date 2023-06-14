/* Reads and displays angle rate values from the LSM6DS3 gyroscopes.

  The Arduino Uno WiFi Rev2 has an onboard LSM6DS3.

  Start the Serial Monitor using Tools > Serial Monitor.
  Run the program view sensor values.
  Optionally use the Serial Plotter.

  W. Newhall 6/2023 (original)
*/

// Recommend not changing these values
#define LOOP_TIME_INTERVAL_MS     200.0   // Step time interval for control loop (200.0 recommended)
#define SERIAL_BAUD_RATE          115200  // Baud rate for serial output (for Serial Monitor and Plotter)

// Include other code used for this program
#include <Arduino_LSM6DS3.h>

// Global variables used across functions
float angle_rate_x_degpsec = 0;  // Rate of angle around x axis (g)
float angle_rate_y_degpsec = 0;  // Rate of angle around y axis (g)
float angle_rate_z_degpsec = 0;  // Rate of angle around z axis (g)

// =======================================================================================================
// MAIN ARDUINO SETUP - This code runs once at startup
// =======================================================================================================
void setup() {
  Serial.begin(SERIAL_BAUD_RATE); // Initialize the serial port for the Serial Monitor

  // Initialize the LSM6DS3 IMU
  if (IMU.begin()) {
    Serial.println("LSM6DS3 IMU successfully initialized.");
  } 
  else {
    Serial.println("LSM6DS3 IMU initialization failed.");
    IMU.end();
    while(1);    // Wait here if IMU initialization failed
  }
}

// =======================================================================================================
// MAIN ARDUINO LOOP - This code runs repeatedly
// =======================================================================================================
void loop() {
  
  if(IMU.gyroscopeAvailable()) { // Check if new gyro values are available
    IMU.readGyroscope(angle_rate_x_degpsec, angle_rate_y_degpsec, angle_rate_z_degpsec);  // Read gyro values
  }

  // Outputs text to serial port for monitoring values using SERIAL PLOTTER
  Serial.print("AngleRateX_degpsec:"); Serial.print(angle_rate_x_degpsec); Serial.print("   ");
  Serial.print("AngleRateY_degpsec:"); Serial.print(angle_rate_y_degpsec); Serial.print("   ");
  Serial.print("AngleRateZ_degpsec:"); Serial.print(angle_rate_z_degpsec); Serial.print("   ");
  Serial.print("\n");

  delay(LOOP_TIME_INTERVAL_MS);   // Delay before looping again
}
