/* Example code for pulse width modulation (PWM)

    Produces a PWM output.  Duty cycle and output pin are hard-coded below.

    Start the Serial Monitor using Tools > Serial Monitor to see status output.
    Use 115,200 baud for Serial Monitor.

  W. Newhall 5/2023 (original version)
*/

#define PWM_OUTPUT_PIN    10        // This will be the pin for the PWM output signal

// This code runs once at startup
void setup() {
  byte duty_cycle;                  // Declare an unsigned byte to use for duty cycle (0 to 255)
  Serial.begin(115200);             // Initialize the serial port for the Serial Monitor
  pinMode(PWM_OUTPUT_PIN, OUTPUT);  // Set the pin to be an output
  duty_cycle = 100;                 // Duty cycle (valid range is 0 to 255)

  analogWrite(PWM_OUTPUT_PIN, duty_cycle );     // Output a PWM signal on the PWM_OUTPUT_PIN

  Serial.print("Creating PWM output on pin ");  // Output descriptive text to the Serial Monitor
  Serial.print(PWM_OUTPUT_PIN);
  Serial.print(" with a duty cicle of ");
  Serial.println(duty_cycle);
}

// This code runs repeatedly
void loop() {
  // There is nothing to run repeatedly for this program
}
