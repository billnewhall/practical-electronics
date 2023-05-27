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
  int vtemp_adc_out;  // ADC measurement of Vtemp in quantization levels
  float vtemp_V;      // ADC measurement of Vtemp in volts (V)
  float temp_C;       // Calculated temperature (C)
  float temp_F;       // Calculated temperature (F)
 
  // Read the ADC value and calculate the measured Vtemp value
  vtemp_adc_out = analogRead(A3);             // Get the integer value from the ADC measuring Vtemp
  vtemp_V = (float)vtemp_adc_out / 1023.0 * 1.1; // Vcca value using 10-bit ADC and 1.1V reference (V)
  
  // Calculate the temperature using the voltage
  // For TMP36, offset is 0.5V, and slope is 10mV/C
  temp_C = 100 * (vtemp_V - 0.5); // Derived from Vtemp = 0.5 + 0.01*Tc for TMP36
  temp_F = 1.8 * temp_C + 32;     // Convert C to F
  // Show the results on the serial monitor
  Serial.print("vtemp_adc_out:"); Serial.print(vtemp_adc_out); Serial.print("  ");
  Serial.print("vtemp_V:"); Serial.print(vtemp_V); Serial.print("  ");
  Serial.print("temp_C:"); Serial.print(temp_C); Serial.print("  ");
  Serial.print("temp_F:"); Serial.print(temp_F); Serial.print("  ");

  Serial.print("\n");

  delay(1000);  // Delay before showing another result
}

