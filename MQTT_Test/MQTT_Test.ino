/* Connects to WiFi and tests publishing and subscribing to an MQTT broker.

  Put your WiFi network name (SSID) and WiFi password in the arduino_secrets.h file.
  This program works with 2.4 GHz WiFi networks only (not 5 GHz).

  Required libraries (install using IDE Library Manager if not yet installed):
    - WiFiNINA by Arduino (see https://www.arduino.cc/reference/en/libraries/wifinina/)
    - PubSubClient by Nick O’Leary (see https://www.arduino.cc/reference/en/libraries/pubsubclient/) 

  How to use:  
    Specify WiFi network (2.4 GHz only) username and password in arduino_secrets.h file.
    Specify MQTT broker.
    Use an MQTT client on a PC to publish to the INPUT_TOPIC below, and the message will display on the Serial Monitor.
    Use an MQTT client on a PC to subscribe to the OUTPUT_TOPIC below, and view the message sent by this program.
    
  W. Newhall 2/2023
*/

// In the next #include line, put the path and filename of your secrets file that contains
// your WiFi network's name (SSID) and WiFi password.  The file should contain the lines:
//    #define WIFI_SSID       "your WiFi network name in quotes"
//    #define WIFI_PASSWORD   "your WiFi network password in quotes"
#include "arduino_secrets.h" 

#define SERIAL_BAUD_RATE    115200                  // Set to the baud rate you want to use (e.g., 115200)
#define MQTT_BROKER         "test.mosquitto.org"    // MQTT broker address (e.g., "test.mosquitto.org" or "192.168.1.002")
#define OUTPUT_TOPIC        "from microcontroller"  // This program will publish to this topic
#define INPUT_TOPIC         "to microcontroller"    // This program will subsribe to this topic

#include "wifi_simple.h"      // Functions for connecting to a WiFi network and printing info
WiFiSimple wifi_simple;       // Provides functions that simplify using WiFi

#include "mqtt_simple.h"      // Functions for using MQTT
MQTTSimple mqtt_simple;       // Provides functions that simplify using MQTT

// =================================================================================================
// MAIN ARDUINO SETUP - This code runs once at startup
// =================================================================================================
void setup() {
  Serial.begin(SERIAL_BAUD_RATE); // Start serial communications for Serial Monitor
  wifi_simple.setupWifi(WIFI_SSID, WIFI_PASSWORD); // Connect to a WiFi network
  wifi_simple.printNetworkInfo();                  // Print information about the WiFi network
  
  // Connect to MQTT broker and subscribe to a topic
  mqtt_simple.setupMQTT(MQTT_BROKER, wifi_simple.getMacAddrString() /* MQTT client ID */);
  mqtt_simple.subscribe(INPUT_TOPIC);     // Subscribe to a topic
  Serial.print("Subscribed to topic:  ");
  Serial.println(INPUT_TOPIC);
}

// =================================================================================================
// MAIN ARDUINO LOOP - This code runs repeatedly
// =================================================================================================
void loop() {
  static float received_msg_value = 0;
  char msg_pub[255];    // Message to publish
  static unsigned int pub_num = 0;  // Publihsed message counter

  mqtt_simple.serviceMQTT();  // Process incoming messages and maintain server connection

  // Display any received message and value
  if(mqtt_simple.getReceivedMsgValue(&received_msg_value)) {  // Check if new msg received
    Serial.print("Value received: ");     
    Serial.println(received_msg_value); // If new msg received, show the converted numeric value
  }

  // Publish a message
  pub_num = pub_num + 1;  // Increment the publication number
  sprintf(msg_pub, "Hello MQTT World [%d]!", pub_num);
  mqtt_simple.publish(OUTPUT_TOPIC, msg_pub);  // Publish to a topic
  Serial.print("Published message \"");
  Serial.print(msg_pub);
  Serial.print("\" to topic \"");
  Serial.print(OUTPUT_TOPIC);
  Serial.println("\"");

  delay(2000);
}
