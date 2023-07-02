/* Provides simplified functions for using WiFi on the Arduino

  W. Newhall 2/2023
*/

#ifndef WIFI_SIMPLE_H
#define WIFI_SIMPLE_H

#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>         // Library for connecting to WiFi networks

class WiFiSimple {
  public:
    WiFiSimple(void);
    void setupWifi(char* ssid, char* password);
    void printNetworkInfo ();
    void printMacAddress(byte *);
    void printRSSI();
    void printWiFiStatus();
    char* getMacAddrString();
    char mac_address_str_[13];
};

#endif // WIFI_SIMPLE_H