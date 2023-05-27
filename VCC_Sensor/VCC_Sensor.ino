/* Reads and shows values for the testbed system sensors.

  Start the Serial Monitor using Tools > Serial Monitor.
  Run the program, and use the Serial Monitor show sensor values.

  W. Newhall 5/2023 (original)
*/

// Defined values
#define SERIAL_BAUD_RATE          115200  // Baud rate for serial output (for Serial Monitor and Plotter)

// =======================================================================================================
// MAIN ARDUINO SETUP - This code runs once at startup
// =======================================================================================================
void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD_RATE);   // Initialize the serial port for the Serial Monitor
  analogReference(INTERNAL1V1);     // Use the 1.1V reference for the ADC
}

// =======================================================================================================
// MAIN ARDUINO LOOP - This code runs repeatedly
// =======================================================================================================
void loop() {
  int vcc_adc_out;  // ADC measurement in quantization levels
  float vcca_V;     // ADC measurement in volts (V)
  float vcc_V;      // Calculated VCC in volts (V)

  // Read the ADC value and calculate the measured VCC value
  vcc_adc_out = analogRead(A0);             // Get the integer value from the ADC
  vcca_V = (float)vcc_adc_out / 1023 * 1.1; // VCCmeas value using 10-bit ADC and 1.1V reference (V)
  vcc_V = vcca_V * (10000 + 1000) / 1000;   // Calculate VCC (the input to voltage divider) (V)  

  // Show the results on the serial monitor
  Serial.print("vcc_adc_out:"); Serial.print(vcc_adc_out); Serial.print("  ");
  Serial.print("vcca_V:"); Serial.print(vcca_V); Serial.print("  ");
  Serial.print("vcc_V:"); Serial.print(vcc_V); Serial.print("  ");
  Serial.print("\n");

  delay(1000);  // Delay before showing another result
}

