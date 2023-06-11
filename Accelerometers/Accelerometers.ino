/* Reads and displays accelerometer values from the LSM6DS3.  

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
  float accel_x_g = 0;  // Acceleration in x axis (g)
  float accel_y_g = 0;  // Acceleration in y axis (g)
  float accel_z_g = 0;  // Acceleration in z axis (g)
  
  IMU.readAcceleration(accel_x_g, accel_y_g, accel_z_g);  // Read accelerometer values

  // Outputs text to serial port for monitoring values using SERIAL PLOTTER
  Serial.print("AccelX_g:"); Serial.print(accel_x_g); Serial.print("   ");
  Serial.print("AccelY_g:"); Serial.print(accel_y_g); Serial.print("   ");
  Serial.print("AccelZ_g:"); Serial.print(accel_z_g); Serial.print("   ");
  Serial.print("\n");

  delay(LOOP_TIME_INTERVAL_MS);   // Delay before looping again
}
