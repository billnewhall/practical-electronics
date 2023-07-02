/* Constant speed prop testbed controlled over IoT using MQTT.

  Put your WiFi network name (SSID) and WiFi password in the arduino_secrets.h file.
  This program works with 2.4 GHz WiFi networks only (not 5 GHz).

  Required libraries (install using IDE Library Manager if not yet installed):
    - WiFiNINA by Arduino (see https://www.arduino.cc/reference/en/libraries/wifinina/)
    - PubSubClient by Nick O’Leary (see https://www.arduino.cc/reference/en/libraries/pubsubclient/) 

  How to use:  
    Specify WiFi network (2.4 GHz only) username and password in arduino_secrets.h file.
    Specify MQTT broker.
    Use an MQTT client on a PC to publishan RPM value to the SUBSCRIBE_TOPIC below, and this will command the specified RPM.
    Use an MQTT client on a PC to subscribe to the PUBLISH_TOPIC below, and view the testbed output values.
  
  W. Newhall 2/2023 (original)
*/

// In the next #include line, put the path and filename of your secrets file that contains
// your WiFi network's name (SSID) and WiFi password.  The file should contain the lines:
//    #define WIFI_SSID       "your WiFi network name in quotes"
//    #define WIFI_PASSWORD   "your WiFi network password in quotes"
#include "arduino_secrets.h" 

// Set these values to your particular configuration
#define PWM_OUTPUT_PIN      10            // This is the pin to output PWM
#define BLADE_SENSOR_PIN    2             // Pin on which to detect interrupt for blade sensor
#define NUM_PROP_BLADES     3             // Number of blades per prop
#define SERIAL_BAUD_RATE    115200        // Baud rate for serial output (for Serial Monitor and Plotter)
#define MQTT_BROKER         "10.0.0.100"  // MQTT broker address (e.g., "10.0.0.100")
#define PUBLISH_TOPIC       "/newhall/from microcontroller"  // This program will publish to this topic
#define SUBSCRIBE_TOPIC     "/newhall/to microcontroller"    // This program will subsribe to this topic

// Recommend not changing these values
#define LOOP_TIME_INTERVAL_MS     200.0   // Step time interval for control loop (200.0 recommended)

// Include other code used for this program
#include "tachometer.h"                   // Implements a tachometer

#include "wifi_simple.h"      // Functions for connecting to a WiFi network and printing info
WiFiSimple wifi_simple;       // Provides functions that simplify using WiFi

#include "mqtt_simple.h"      // Functions for using MQTT
MQTTSimple mqtt_simple;       // Provides functions that simplify using MQTT

// Global variables used across functions
Tachometer Tach(NUM_PROP_BLADES, BLADE_SENSOR_PIN); // Calculates prop speed based on sensor input
PropSpeed prop_speed;         // Stores RPM, blade perod, and blade frequency
static float desired_rpm = 0; // Desired RPM (setpoint)
static float duty_cycle = 0;  // Duty cycle for propeller motor (0-255)
static float error_rpm = 0;   // Difference between desired RPM and measured RPM
float vcc_V;                  // Calculated VCC in volts (V)
float vbatt_V;                // Calculated Vbatt in volts (V)
float vsm_V;                  // Calculated Vbatt in volts (V)
float temp_F;                 // Temperature sensor (F)
char output_text[255];        // Text to output on the serial monitor and to publish via MQTT
char field_text[32];          // Text for label and value for one of the fields
char val_str[32];             // A place to convert float to string since %f doesn't work on Arduino

// =======================================================================================================
// MAIN ARDUINO SETUP - This code runs once at startup
// =======================================================================================================
void setup() {
  Serial.begin(SERIAL_BAUD_RATE); // Initialize the serial port for the Serial Monitor
  analogReference(INTERNAL1V1);   // Use the 1.1V reference for the ADC
  Serial.println("\nEnter an RPM using the serial monitor.");

  // Connect to Wi-Fi network
  wifi_simple.setupWifi(WIFI_SSID, WIFI_PASSWORD); // Connect to a WiFi network
  wifi_simple.printNetworkInfo();                  // Print information about the WiFi network
  
  // Connect to MQTT broker and subscribe to a topic
  mqtt_simple.setupMQTT(MQTT_BROKER, wifi_simple.getMacAddrString() /* MQTT client ID */);
  mqtt_simple.subscribe(SUBSCRIBE_TOPIC);     // Subscribe to a topic
  Serial.print("Subscribed to topic:  ");
  Serial.println(SUBSCRIBE_TOPIC);
}

// =======================================================================================================
// MAIN ARDUINO LOOP - This code runs repeatedly
// =======================================================================================================
void loop() {
  // Declare variables
  float Ki;               // Integration gain constant
  float value_from_mqtt;  // Value received via MQTT (commanded RPM value)

  // This line controls how often the main loop runs (it avoids using delay() function)
  if(!loop_timer_expired(LOOP_TIME_INTERVAL_MS)) return;
    
  // If the user has input a desired RPM, then store that value
  if(Serial.available() > 0) {
    desired_rpm = Serial.parseInt();     // Get the value from the user input
  }

  if(mqtt_simple.getReceivedMsgValue(&value_from_mqtt)) {  // Check if new msg received
    desired_rpm = value_from_mqtt;
    Serial.print("Value received via MQTT: ");     
    Serial.println(desired_rpm); // If new msg received, show the converted numeric value
  }
  mqtt_simple.serviceMQTT();  // Process incoming messages and maintain server connection

  // Get the propeller speed
  prop_speed = Tach.GetPropSpeed();

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

  // Set the PWM duty cycle (0-255) in the microcontroller
  analogWrite(PWM_OUTPUT_PIN, (int)duty_cycle );

// END OF CONTROL LOOP APPROACH
// ********************************************************************************************************
// *****************************************************************************************************

  GetTemperature();
  GetVoltages();
  
  // Output text via the serial port
  OutputTextForSerialPlotter();   // Get values for Serial Monitor, Serial Plotter, and MQTT message
  Serial.println(output_text);    // Show the values on the Serial Monitor
  mqtt_simple.publish(PUBLISH_TOPIC, output_text);    // Publish the values via MQTT
}

// =======================================================================================================
// SUPPORTING FUNCTIONS - Recommend not changing this code
// =======================================================================================================

// Outputs text to serial port for monitoring values using SERIAL PLOTTER
void OutputTextForSerialPlotter() {
  sprintf(output_text, "");
  sprintf(field_text, "DutyCycle:%s   ", dtostrf(duty_cycle, 0, 2, val_str));
  strcat(output_text, field_text);
  sprintf(field_text, "DesiredRPM:%s   ", dtostrf(desired_rpm, 0, 0, val_str));
  strcat(output_text, field_text);
  sprintf(field_text, "MeasuredRPM:%s   ", dtostrf(prop_speed.rpm, 0, 0, val_str));
  strcat(output_text, field_text);
  sprintf(field_text, "Error:%s   ", dtostrf(error_rpm, 0, 0, val_str));
  strcat(output_text, field_text);
  sprintf(field_text, "Vcc_V:%s   ", dtostrf(vcc_V, 0, 2, val_str));
  strcat(output_text, field_text);
  sprintf(field_text, "Vbatt_V:%s   ", dtostrf(vbatt_V, 0, 2, val_str));
  strcat(output_text, field_text);
  sprintf(field_text, "Vsm_V:%s   ", dtostrf(vsm_V, 0, 2, val_str));
  strcat(output_text, field_text);
  sprintf(field_text, "MotorTemp_F:%s   ", dtostrf(temp_F, 0, 2, val_str));
  strcat(output_text, field_text);
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

void GetVoltages(void) {
  int vcca_adc_out;   // ADC measurement of Vcca in quantization levels
  float vcca_V;       // ADC measurement of Vcc in volts (V)
  int vbatta_adc_out; // ADC measurement of Vbatta in quantization levels
  float vbatta_V;     // ADC measurement of Vbatta in volts (V)
  int vsma_adc_out;   // ADC measurement of Vsma in quantization levels
  float vsma_V;       // ADC measurement of Vbatt in volts (V)

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

  return;
}

void GetTemperature(void) {
  int vtemp_adc_out;  // ADC measurement of Vtemp in quantization levels
  float vtemp_V;      // ADC measurement of Vtemp in volts (V)
  float temp_C;       // Calculated temperature (C)
  
  // Read the ADC value and calculate the measured Vtemp value
  vtemp_adc_out = analogRead(A3);             // Get the integer value from the ADC measuring Vtemp
  vtemp_V = (float)vtemp_adc_out / 1023.0 * 1.1; // Vcca value using 10-bit ADC and 1.1V reference (V)
  
  // Calculate the temperature using the voltage
  // For TMP36, offset is 0.5V, and slope is 10mV/C
  temp_C = 100 * (vtemp_V - 0.5); // Derived from Vtemp = 0.5 + 0.01*Tc for TMP36
  temp_F = 1.8 * temp_C + 32;     // Convert C to F
  return;
}

