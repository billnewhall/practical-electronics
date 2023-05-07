/* Tachometer

  Calculates RPM using inputs from a propeller blade-crossing sensor.

  W. Newhall 5/2023 (original)
*/

#include <Arduino.h>
#include "tachometer.h"

volatile unsigned long previous_blade_crossing_us = 0;
volatile unsigned long current_blade_crossing_us = 0;
volatile unsigned long blade_period_us[BLADE_PER_NUM_AVG];

// ==========================================================================
// Interrupt service routine (ISR) set up for blade sensor pin.
// For each blade crossing, record the time since the previous blade crossing
// ==========================================================================
void isr_blade_crossing() {
  static int avg_index = 0;   // Index for averaging
  current_blade_crossing_us = micros();   // Current time (us)
  blade_period_us[avg_index] = current_blade_crossing_us - previous_blade_crossing_us; 
  if(++avg_index == BLADE_PER_NUM_AVG) {  // Index in array to store next blade crossing period
    avg_index = 0;
  }
  previous_blade_crossing_us = current_blade_crossing_us;   // Previous crossing is now current crossing
}

// ==========================================================================
// Tachometer class definition
// ==========================================================================

Tachometer::Tachometer(int num_prop_blades, int blade_sensor_pin) {
  num_prop_blades_ = num_prop_blades;
  blade_sensor_pin_ = blade_sensor_pin;
  max_blade_time_us_ = MAX_BLADE_TIME;          // Below this time between blade crossings, consider RPM to be zero
  avg_blade_period_us_ = 0;
  blade_freq_hz_ = 0;
  pinMode(blade_sensor_pin_, INPUT);    // The interrupt pin is an input
  attachInterrupt(digitalPinToInterrupt(blade_sensor_pin_), isr_blade_crossing, FALLING);     // Set up the interrupt
}

// Get the average blade period
// Populates avg_blade_period_us_ with latest value
float Tachometer::GetAvgBladePeriod(void) {
  int n;
  unsigned long avg_blade_period_us = 0;        // Average blade period result (us)
  unsigned long time_since_last_blade_us;       // Time since previous blade crossing (us)
  unsigned long current_time_us;
  unsigned long local_previous_blade_crossing_us; // Get prev cross time first so that it won't change if interrupt during this function

  // If a blade has not crossed the sensor in MAX_BLADE_TIME us, then consider
  // the blade period to be zero.
  local_previous_blade_crossing_us = previous_blade_crossing_us;  // Get prev cross time first so that it won't change if interrupt during this function
  current_time_us = micros();   // ... then get current time
  time_since_last_blade_us = current_time_us - local_previous_blade_crossing_us;
  if(time_since_last_blade_us <= max_blade_time_us_) {
    for(n=0; n<BLADE_PER_NUM_AVG; n++) {          // Loop through all the values to average
      avg_blade_period_us_ += blade_period_us[n];  // Sum all the values to average
    }
    avg_blade_period_us_ = avg_blade_period_us_ / (float)BLADE_PER_NUM_AVG;  // Calculate the average
  }
  else {
    avg_blade_period_us_ = 0;  // Consider blade period to be zero
    // FOR DEBUG
    /* Serial.println("");
    Serial.print("current_time_us: ");
    Serial.print(current_time_us);
    Serial.print(", previous_blade_crossing_us: ");
    Serial.print(previous_blade_crossing_us);
    Serial.print(", local_previous_blade_crossing_us: ");
    Serial.print(previous_blade_crossing_us);
    Serial.print(", local_time_since_last_blade_us: ");
    Serial.print(previous_blade_crossing_us);
    Serial.println("");
    // END DEBUG
	*/
  }
  return avg_blade_period_us_;
}

// Calculate the  blade crossing frequency from the blade
// Populates blade_freq_hz_ and avg_blade_period_us_ with latest values
float Tachometer::GetFreq(void) {
  long int avg_blade_period_us;
  float freq_Hz;

  GetAvgBladePeriod(); // Get the blade period
  if(avg_blade_period_us_ > 0) // If blade period is greater than zero, calculate freq
  {
    blade_freq_hz_ = 1000000.0/avg_blade_period_us_;
  }
  else  // If the blade period is zero, then consider frequency to be zero
  {
    blade_freq_hz_ = 0;
  }
  return blade_freq_hz_;
}

// Calculate the RPM from the blade crossing frequency
// Populates rpm_, blade_freq_hz_, and avg_blade_period_us_ with latest values
float Tachometer::GetRpm(void) {
  rpm_ = GetFreq() * 60.0 / (float)num_prop_blades_;
  return rpm_;
}

// Get the RPM, frequency, and average blade period all at once in one structure
PropSpeed Tachometer::GetPropSpeed(void) {
  PropSpeed prop_speed;
  GetRpm();
  prop_speed.rpm = rpm_;
  prop_speed.blade_period_us = avg_blade_period_us_;
  prop_speed.blade_freq_hz = blade_freq_hz_;
  
  return prop_speed;
}
