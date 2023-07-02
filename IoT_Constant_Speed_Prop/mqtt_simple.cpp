/* Provides simplified functions for using MQTT on the Arduino

  Required library:  PubSubClient (see https://www.arduino.cc/reference/en/libraries/pubsubclient/)
  If you don't have this library installed, install using this process:
    Open the library manager using Tools > Manage Libraries...
    In the "Filter your search..." box, type pubsubclient
    Scroll down to find "PubSubClient by Nick O'Leary", and click install.
    
  PubSubClient library API documentation is here:  https://pubsubclient.knolleary.net/api

  W. Newhall 2/2023
*/

#include "mqtt_simple.h"

WiFiClient wifi_client;
PubSubClient  mqtt_client(wifi_client);
float received_msg_value = 0;
bool value_is_new = false;

// Callback that handles received messages from the subscribed topic
void mqtt_received_message_callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Make sure there is a termination when converting to string
  String str_payload((char*)payload); // Convert payload to a String to then convert to float
  received_msg_value = str_payload.toFloat();   // Convert to float
  value_is_new = true;
  Serial.print("Received message \"");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.print("\" from topic \"");
  Serial.print(topic);
  Serial.println("\"");

  // Serial.print("Message arrived on topic \"");
  // Serial.print(topic);
  // Serial.print("\": ");
  // for (int i=0;i<length;i++) {
  //   Serial.print((char)payload[i]);
  // }
  // Serial.println();
}

MQTTSimple::MQTTSimple() {
}

void MQTTSimple::setupMQTT(char* mqtt_broker_address, char* mqtt_client_id) {
  mqtt_broker_address_ = mqtt_broker_address;
  mqtt_client_id_ = mqtt_client_id;
  mqtt_client.setServer(mqtt_broker_address, 1883);
  mqtt_client.setCallback(mqtt_received_message_callback);
  if (!mqtt_client.connected()) {
    reconnect();
  }
}

void MQTTSimple::serviceMQTT(void) {
  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop();
}

// Reconnect to MQTT client
void MQTTSimple::reconnect() {
  unsigned long nextTime = 0;
  
  while (!mqtt_client.connected()) { // Loop until we're reconnected
    Serial.print("Connecting to MQTT broker: ");
    Serial.println(mqtt_broker_address_);
    Serial.print("MQTT client ID is: ");
    Serial.println(mqtt_client_id_);
    // Attempt to connect
    if (mqtt_client.connect(mqtt_client_id_)) {
      Serial.print("Connected to MQTT broker: ");
      Serial.println(mqtt_broker_address_);
    } 
    else {
      Serial.println("Waiting 5 sec to attempt reconnect to MQTT broker.");
      // Wait 5 seconds before retrying
      nextTime = millis() + 5000;   // Next time to try to connect
      while(millis() < nextTime) {
      }
    }
  }
}

void MQTTSimple::subscribe(char* topic) {
  mqtt_client.subscribe(topic);
}

void MQTTSimple::publish(char* topic, char* message) {
  mqtt_client.publish(topic, message);
}

bool MQTTSimple::getReceivedMsgValue(float * msg_value) {
  bool retval;
  *msg_value = received_msg_value;
  retval = value_is_new;
  value_is_new = false;
  return retval;
}
