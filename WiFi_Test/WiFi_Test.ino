/* Tests connecting to a WiFi network and prints connection information to the Serial Monitor.

  Put your WiFi network name (SSID) and WiFi password in the arduino_secrets.h file.
  This program works with 2.4 GHz WiFi networks only (not 5 GHz).

  The library WiFiNINA by Arduino is required (install using IDE Library Manager).

  W. Newhall 2/2023
*/

// In the next #include line, put the path and filename of your secrets file that contains
// your WiFi network's name (SSID) and WiFi password.  The file should contain the lines:
//    #define WIFI_SSID       "put your WiFi network name in quotes"
//    #define WIFI_PASSWORD   "put your WiFi network password in quotes"
#include "arduino_secrets.h" 

#define SERIAL_BAUD_RATE   115200             // Set to the baud rate you want to use (e.g., 115200)
//#define NETWORK_HOST_NAME  "arduino_testbed"  // A hostname for your Arduino on the network

#include "wifi_simple.h"      // Functions for connecting to a WiFi network and printing info
WiFiSimple wifi_simple;       // Provides functions that simplify using WiFi

// =================================================================================================
// MAIN ARDUINO SETUP - This code runs once at startup
// =================================================================================================
void setup() {
  Serial.begin(SERIAL_BAUD_RATE); // Start serial communications for Serial Monitor
  //wifi_simple.setupWifi(WIFI_SSID, WIFI_PASSWORD, NETWORK_HOST_NAME); // Connect to a WiFi network
  wifi_simple.setupWifi(WIFI_SSID, WIFI_PASSWORD);  // Connect to a WiFi network
  wifi_simple.printNetworkInfo();                   // Print information about the WiFi network
}

// =================================================================================================
// MAIN ARDUINO LOOP - This code runs repeatedly
// =================================================================================================
void loop() {
  wifi_simple.printRSSI();        // Print the receive signal strength 
  Serial.print(", ");             // Separate RSSI and status in output text
  wifi_simple.printWiFiStatus();  // Print the WiFi status
  Serial.println();               // New line
  delay(1000);                    // Delay and repeat
}

