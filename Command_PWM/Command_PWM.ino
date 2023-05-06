/* Creates a pulse width modulated (PWM) signal based on the duty cycle
   entered by the user

  Start the Serial Monitor using Tools > Serial Monitor.
  Run the program, and use the Serial Monitor to see a prompt.
  When prompted, use the Serial Monitor to enter a duty cycle.

  W. Newhall 5/2023 (original version)
*/

#define PWM_OUTPUT_PIN    10        // This is the pin to output PWM

// This code runs once at startup
void setup() {
  Serial.begin(115200);             // Initialize the serial port for the Serial Monitor
  pinMode(PWM_OUTPUT_PIN, OUTPUT);  // Set the pin to be an output
}

// This code runs repeatedly
void loop() {
  // Declare variables
  int duty_cycle;   // Duty cycle value (0 to 255)

  // Show a prompt and get user input
  Serial.println("\nEnter a PWM duty cycle value (0-255) using the serial monitor.");
  while (Serial.available() == 0) { } // Wait until user enters a duty cycle value
  duty_cycle = Serial.parseInt();     // Get the integer value from the user input
  Serial.print("Setting PWM duty cycle value (0-255) to: ");
  Serial.println(duty_cycle);

  // Set the PWM duty cycle using the value entered by the user
  analogWrite(PWM_OUTPUT_PIN, duty_cycle );
}
