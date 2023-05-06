/* Tachometer

  Calculates RPM using inputs from a propeller blade-crossing sensor.
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
  pinMode(blade_sensor_pin_, INPUT);    // The interrupt pin is an input
  attachInterrupt(digitalPinToInterrupt(blade_sensor_pin_), isr_blade_crossing, FALLING);     // Set up the interrupt
}

// Get the average blade period
float Tachometer::GetAvgBladePeriod(void) {
  int n;
  unsigned long avg_blade_period_us = 0;        // Average blade period result (us)
  unsigned long time_since_last_blade_us;       // Time since previous blade crossing (us)

  // If a blade has not crossed the sensor in MAX_BLADE_TIME us, then consider
  // the blade period to be zero.
  time_since_last_blade_us = micros() - previous_blade_crossing_us;
  if(time_since_last_blade_us < max_blade_time_us_) {
    for(n=0; n<BLADE_PER_NUM_AVG; n++) {          // Loop through all the values to average
      avg_blade_period_us += blade_period_us[n];  // Sum all the values to average
    }
    avg_blade_period_us = avg_blade_period_us / BLADE_PER_NUM_AVG;  // Calculate the average
  }
  else {
    avg_blade_period_us = 0;  // Consider blade period to be zero
  }
  return avg_blade_period_us;
}

// Calculate the blade crossing frequency from the blade period
float Tachometer::GetFreq(void) {
  long int avg_blade_period_us;
  float freq_Hz;

  avg_blade_period_us = GetAvgBladePeriod(); // Get the blade period
  if(avg_blade_period_us > 0) // If blade period is greater than zero, calculate freq
  {
    freq_Hz = 1000000.0/avg_blade_period_us;
  }
  else  // If the blade period is zero, then consider frequency to be zero
  {
    freq_Hz = 0;
  }
  return freq_Hz;
}

// Calculate the RPM from the blade crossing frequency
float Tachometer::GetRpm(void) {
  float tachometer_rpm;
  tachometer_rpm = GetFreq() * 60 / num_prop_blades_;
  return tachometer_rpm;
}
