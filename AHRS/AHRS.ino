/* Implements an AHRS using the LSM6DS3 accelerometers and gyroscopes.

  The Arduino Uno WiFi Rev2 has an onboard LSM6DS3.

  Start the Serial Monitor using Tools > Serial Monitor.
  Run the program view sensor values.
  Optionally use the Serial Plotter.

  See https://www.arduino.cc/reference/en/libraries/madgwick/ as a reference
  for the Madwick algorithm library.

  W. Newhall 6/2023 (original)
*/

// Recommend not changing these values
#define LOOP_TIME_INTERVAL_MS     200.0   // Step time interval for control loop (200.0 recommended)
#define SERIAL_BAUD_RATE          115200  // Baud rate for serial output (for Serial Monitor and Plotter)

// Include other code used for this program
#include <Arduino_LSM6DS3.h>
#include <MadgwickAHRS.h>

// Global variables used across functions
float accel_x_g = 0;              // Acceleration in x axis (g)
float accel_y_g = 0;              // Acceleration in y axis (g)
float accel_z_g = 0;              // Acceleration in z axis (g)
float angle_rate_x_degpsec = 0;   // Rate of angle around x axis (g)
float angle_rate_y_degpsec = 0;   // Rate of angle around y axis (g)
float angle_rate_z_degpsec = 0;   // Rate of angle around z axis (g)
float roll_x_deg = 0;               // Roll angle (deg)
float pitch_y_deg = 0;              // Pitch angle (deg)
float yaw_z_deg = 0;                // Yaw angle (deg)

Madgwick mw_filter;  // Madgwick algorithm for roll, pitch, and yaw calculations

// =======================================================================================================
// MAIN ARDUINO SETUP - This code runs once at startup
// =======================================================================================================
void setup() {
  float ahrs_update_rate_hz; // Rate at which ARHS values are updated

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

  // Initialize the Madgwick filter
  ahrs_update_rate_hz = 1000.0/(float)LOOP_TIME_INTERVAL_MS;
  mw_filter.begin(ahrs_update_rate_hz);
}

// =======================================================================================================
// MAIN ARDUINO LOOP - This code runs repeatedly
// =======================================================================================================
void loop() {
  if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) { // Check if new accel and gyro values are available
    IMU.readAcceleration(accel_x_g, accel_y_g, accel_z_g);
    IMU.readGyroscope(angle_rate_x_degpsec, angle_rate_y_degpsec, angle_rate_z_degpsec);  // Read gyro values
    mw_filter.updateIMU(angle_rate_x_degpsec, angle_rate_y_degpsec, angle_rate_z_degpsec, 
      accel_x_g, accel_y_g, accel_z_g); // Update the roll, putch, and yaw values
    roll_x_deg = mw_filter.getRoll();     // Get the roll angle from the filter (deg)
    pitch_y_deg = mw_filter.getPitch();   // Get the pitch angle from the filter (deg)
    yaw_z_deg = mw_filter.getYaw();       // Get the yaw angle from the filter (deg)
  }

  // Outputs text to serial port for monitoring values using SERIAL PLOTTER
  Serial.print("Roll_X_deg:"); Serial.print(roll_x_deg, 0); Serial.print("   ");
  Serial.print("Pitch_Y_deg:"); Serial.print(pitch_y_deg, 0); Serial.print("   ");
  Serial.print("Yaw_Z_deg:"); Serial.print(yaw_z_deg, 0); Serial.print("   ");
  Serial.print("AccelX_g:"); Serial.print(accel_x_g); Serial.print("   ");
  Serial.print("AccelY_g:"); Serial.print(accel_y_g); Serial.print("   ");
  Serial.print("AccelZ_g:"); Serial.print(accel_z_g); Serial.print("   ");
  // For debugging or optional display:
    // Serial.print("AngleRateX_degpsec:"); Serial.print(angle_rate_x_degpsec); Serial.print("   ");
    // Serial.print("AngleRateY_degpsec:"); Serial.print(angle_rate_y_degpsec); Serial.print("   ");
    // Serial.print("AngleRateZ_degpsec:"); Serial.print(angle_rate_z_degpsec); Serial.print("   ");
  Serial.print("\n");

  delay(LOOP_TIME_INTERVAL_MS);   // Delay before looping again
}
