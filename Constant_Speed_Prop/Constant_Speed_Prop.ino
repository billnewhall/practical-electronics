/* Implements closed-loop propeller speed control.

  Start the Serial Monitor using Tools > Serial Monitor.
  Run the program, and use the Serial Monitor to enter prop RPM values.
  Optionally use the Serial Plotter (see comments in code below).

  W. Newhall 2/2023 (original)
*/

// Set these values to your particular configuration
#define PWM_OUTPUT_PIN            10      // This is the pin to output PWM
#define BLADE_SENSOR_PIN          2       // Pin on which to detect interrupt for blade sensor
#define NUM_PROP_BLADES           3       // Number of blades per prop

// Recommend not changing these values
#define LOOP_TIME_INTERVAL_MS     200.0   // Step time interval for control loop (200.0 recommended)
#define SERIAL_BAUD_RATE          115200  // Baud rate for serial output (for Serial Monitor and Plotter)

// Include other code used for this program
#include "tachometer.h"                   // Implements a tachometer

// Global variables used across functions
Tachometer Tach(NUM_PROP_BLADES, BLADE_SENSOR_PIN); // Calculates prop speed based on sensor input
PropSpeed prop_speed;                     // Stores RPM, blade perod, and blade frequency
static float desired_rpm = 0;             // Desired RPM (setpoint)
static float duty_cycle = 0;              // Duty cycle for propeller motor (0-255)
static float error_rpm = 0;               // Difference between desired RPM and measured RPM

// =======================================================================================================
// MAIN ARDUINO SETUP - This code runs once at startup
// =======================================================================================================
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);                   // Initialize the serial port for the Serial Monitor
  Serial.println("\nEnter an RPM using the serial monitor.");
}

// =======================================================================================================
// MAIN ARDUINO LOOP - This code runs repeatedly
// =======================================================================================================
void loop() {
  // Declare variables
  float Ki; // Integration gain constant

  // This line controls how often the main loop runs (it avoids using delay() function)
  if(!loop_timer_expired(LOOP_TIME_INTERVAL_MS)) return;
    
  // If the user has input a desired RPM, then store that value
  if(Serial.available() > 0) {
    desired_rpm = Serial.parseInt();     // Get the value from the user input
  }

  // Set the PWM duty cycle (0-255) in the microcontroller
  analogWrite(PWM_OUTPUT_PIN, (int)duty_cycle );

  // Get the propeller speed
  prop_speed = Tach.GetPropSpeed();

  // Output text via the serial port
  OutputTextForSerialPlotter();   // Watch values on the Serial Plotter
  
  // *****************************************************************************************************
  // *****************************************************************************************************
  // THIS IS WHERE YOU IMPLEMENT THE CONTROL LOOP APPROACH AND UPDATE THE DUTY CYCLE

  // Control loop gain
  Ki = 0.01;     // Set the integration gain constant here

  // Control loop calculations
  error_rpm = desired_rpm - prop_speed.rpm; // Difference between desired and measured RPM
  duty_cycle = duty_cycle + Ki * error_rpm; // Integrate the factored error

  // Don't let the duty cycle get out of the range from 0 to 255
  if(duty_cycle < 0) {
    duty_cycle = 0;
  }
  else if (duty_cycle > 255) {
    duty_cycle = 255;
  }

  // If desired RPM is zero, then remove power from the motor
  if(desired_rpm == 0) {                    
    duty_cycle = 0;
  }

// END OF CONTROL LOOP APPROACH
// ********************************************************************************************************
// *****************************************************************************************************
}

// =======================================================================================================
// SUPPORTING FUNCTIONS - Recommend not changing this code
// =======================================================================================================

// Outputs text to serial port for monitoring values using SERIAL PLOTTER
void OutputTextForSerialPlotter() {
  Serial.print("DutyCycle:"); Serial.print(duty_cycle); Serial.print("   ");
  Serial.print("DesiredRPM:"); Serial.print(desired_rpm, 0); Serial.print("   ");
  Serial.print("MeasuredRPM:"); Serial.print(prop_speed.rpm, 0); Serial.print("   ");
  Serial.print("Error:"); Serial.print(error_rpm, 0); Serial.print(" ");
  Serial.print("\n");
}

// Used instead of delay() function to control main loop period
// Returns true if timer has expired and it is time to run the main loop again.
// Returns false if it is not time to run the main loop again.
bool loop_timer_expired(unsigned long loop_interval_ms) {
  static unsigned long previous_millis = 0; // Previous time loop ran
  unsigned long current_millis;             // Current time
  bool retval;                              // Return value

  current_millis = millis();  // Get the current time
  if(current_millis - previous_millis < loop_interval_ms) {
    retval = false;   // Not time to run the main loop again
  }
  else {
    previous_millis = current_millis;
    retval = true;    // It is time to run the main loop again
  }
  return retval;
}