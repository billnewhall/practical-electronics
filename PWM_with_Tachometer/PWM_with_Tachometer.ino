/* Creates a pulse width modulated (PWM) signal for motor speed control,
  and performs rotational speed (RPM) measurement.

  Start the Serial Monitor using Tools > Serial Monitor.
  Run the program, and use the Serial Monitor to enter duty cycle values.
  Timing and RPM information shows on the Serial Monitor.

  W. Newhall 2/2023 (original version)
*/

#define PWM_OUTPUT_PIN    10      // This is the pin to output PWM
#define BLADE_SENSOR_PIN  2       // Pin on which to detect interrupt for blade IR sensor
#define NUM_PROP_BLADES   3       // Number of blades per prop

#include "tachometer.h"           // Implements a tachometer

Tachometer Tach(NUM_PROP_BLADES, BLADE_SENSOR_PIN); // Tachometer object

// This code runs once at startup
void setup() {
  Serial.begin(115200);                     // Initialize the serial port for the Serial Monitor
  Serial.println("\nEnter a PWM duty cycle value (0-255) using the serial monitor.");
}

// This code runs repeatedly
void loop() {
  // Declare variables
  int duty_cycle;       // Duty cycle value (0 to 255)
  long int avg_blade_period_us;
  float freq_Hz;        // Frequency of blades crossing sensor (Hz)
  float tachometer_rpm; // Rotational speed (rpm)
  PropSpeed prop_speed; // Structure for holding prop speed data

  // Show a prompt and get user input
  while (Serial.available() == 0) { } // Wait until user enters a duty cycle value
  duty_cycle = Serial.parseInt();     // Get the integer value from the user input

  // Set the PWM duty cycle using the value entered by the user
  analogWrite(PWM_OUTPUT_PIN, duty_cycle );

  while (Serial.available() == 0) { // Wait until user hits enter
    prop_speed = Tach.GetPropSpeed();
    Serial.print("Duty Cycle (0-255): "); Serial.print(duty_cycle); Serial.print(", ");
    Serial.print("Period: "); Serial.print(prop_speed.blade_period_us); Serial.print(" us, ");
    Serial.print("Frequency: "); Serial.print(prop_speed.blade_freq_hz); Serial.print(" Hz, ");
    Serial.print("Tachometer: "); Serial.print(prop_speed.rpm, 0); Serial.print(" RPM");
    Serial.println("");

    delay(250);
  }
}
