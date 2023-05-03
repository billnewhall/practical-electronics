/* Example code for pulse width modulation (PWM)

    Produces a PWM output.  Duty cycle and output pin are hard-coded below.

    Start the Serial Monitor using Tools > Serial Monitor to see status output.
    Use 115,200 baud for Serial Monitor.

  W. Newhall 5/2023 (original version)
*/

#define PWM_OUTPUT_PIN    10

// This code runs once at startup
void setup() {
  Serial.begin(115200);             // Initialize the serial port for the Serial Monitor
  pinMode(PWM_OUTPUT_PIN, OUTPUT);  // Set the pin to be an output
}

// This code runs repeatedly
void loop() {
  byte duty_cycle;
  duty_cycle = 100;   // Duty cycle (valid range is 0 to 255)
  analogWrite(PWM_OUTPUT_PIN, duty_cycle );   // Output a PWM signal on the PWM_OUTPUT_PIN
}
