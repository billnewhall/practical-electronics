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
  int vcca_adc_out;   // ADC measurement of Vcca in quantization levels
  float vcca_V;       // ADC measurement of Vcc in volts (V)
  float vcc_V;        // Calculated VCC in volts (V)
 
  int vbatta_adc_out; // ADC measurement of Vbatta in quantization levels
  float vbatta_V;     // ADC measurement of Vbatta in volts (V)
  float vbatt_V;      // Calculated Vbatt in volts (V)
 
  int vsma_adc_out;   // ADC measurement of Vsma in quantization levels
  float vsma_V;       // ADC measurement of Vbatt in volts (V)
  float vsm_V;        // Calculated Vbatt in volts (V)

  // Read the ADC value and calculate the measured VCC value
  vcca_adc_out = analogRead(A0);             // Get the integer value from the ADC measuring Vcca
  vcca_V = (float)vcca_adc_out / 1023 * 1.1; // Vcca value using 10-bit ADC and 1.1V reference (V)
  vcc_V = vcca_V * (10000 + 1000) / 1000;    // Calculate Vcc (the input to voltage divider) (V)  

  // Read the ADC value and calculate the measured battery voltage value
  vbatta_adc_out = analogRead(A1);               // Get the integer value from the ADC measuring Vbatta
  vbatta_V = (float)vbatta_adc_out / 1023 * 1.1; // Vbatta value using 10-bit ADC and 1.1V reference (V)
  vbatt_V = vbatta_V * (10000 + 1000) / 1000;    // Calculate Vbatt (the input to voltage divider) (V)  

  // Read the ADC value and calculate the measured motor supply voltage value
  vsma_adc_out = analogRead(A2);             // Get the integer value from the ADC measuring Vsma
  vsma_V = (float)vsma_adc_out / 1023 * 1.1; // Vsma value using 10-bit ADC and 1.1V reference (V)
  vsm_V = vsma_V * (10000 + 1000) / 1000;    // Calculate Vsm (the input to voltage divider) (V)  

  // Show the results on the serial monitor
  Serial.print("vcca_adc_out:"); Serial.print(vcca_adc_out); Serial.print("  ");
  Serial.print("vcca_V:"); Serial.print(vcca_V); Serial.print("  ");
  Serial.print("vcc_V:"); Serial.print(vcc_V); Serial.print("  ");

  Serial.print("vbatta_adc_out:"); Serial.print(vbatta_adc_out); Serial.print("  ");
  Serial.print("vbatta_V:"); Serial.print(vbatta_V); Serial.print("  ");
  Serial.print("vbatt_V:"); Serial.print(vbatt_V); Serial.print("  ");

  Serial.print("vsma_adc_out:"); Serial.print(vsma_adc_out); Serial.print("  ");
  Serial.print("vsma_V:"); Serial.print(vsma_V); Serial.print("  ");
  Serial.print("vsm_V:"); Serial.print(vsm_V); Serial.print("  ");

  Serial.print("\n");

  delay(1000);  // Delay before showing another result
}

