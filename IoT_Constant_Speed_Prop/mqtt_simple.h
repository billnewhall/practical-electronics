/* Provides simplified functions for using MQTT on the Arduino

  W. Newhall 2/2023
*/

#ifndef MQTT_SIMPLE_H
#define MQTT_SIMPLE_H

#include <Arduino.h>
#include <PubSubClient.h>     //
#include <WiFiNINA.h>         // Library for connecting to WiFi networks

class MQTTSimple {
  public:
    MQTTSimple(void);
    void setupMQTT(char* mqtt_broker_address, char* mqtt_client_id);
    void serviceMQTT();                           // Process incoming messages and maintain server connection
    void subscribe(char* topic);                  // Subscribe to a topic
    void publish(char* topic, char* message);     // Publish a message
    bool getReceivedMsgValue(float * msg_value);  // Get the float value of a message (0 msg is not a number)
  private:
    void reconnect();
    char* mqtt_broker_address_;
    char* mqtt_client_id_;
};

#endif // MQTT_SIMPLE_H