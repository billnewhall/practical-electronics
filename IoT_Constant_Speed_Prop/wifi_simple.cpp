/* Provides simplified functions for using WiFi on the Arduino

  W. Newhall 2/2023
*/

#include "wifi_simple.h"

WiFiSimple::WiFiSimple() {
}

// Connect to WiFi network
void WiFiSimple::setupWifi(char* ssid, char* password) {
  Serial.println("\r\nSetting up WiFi");
  // Check if the WiFi module is present
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed. Stopping here.");
    while (true);   // Don't continue
  }
  else {
    Serial.println("WiFi module is present on the Arduino board");
  }

  // Show the WiFi module firmware version
  String fv = WiFi.firmwareVersion();
  Serial.print("Your WiFi firmware version is ");
  Serial.println(fv);
  Serial.print("The latest firmware version is ");
  Serial.println(WIFI_FIRMWARE_LATEST_VERSION);

  // Connect to the WiFi network
  Serial.print("Connecting to WiFi network: ");
  Serial.println(ssid);
  Serial.print("Attempting to connect to WiFi network ");
  WiFi.begin(ssid, password); // Connect to network
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("WiFi successfully connected");
  Serial.print("Arduino IP address: ");
  Serial.println(WiFi.localIP());
}

// Prints MAC addresses (arduino and router) and encryption type to the serial monitor
void WiFiSimple::printNetworkInfo (void) {
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address of Arduino: ");
  printMacAddress(mac);
  Serial.println();

  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("MAC address of router (BSSID): ");
  printMacAddress(bssid);  
  Serial.println();
  
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type: ");
  Serial.println(encryption, HEX);
  Serial.println();
}

// Print a MAC address to the Serial Monitor
void WiFiSimple::printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
}

// Print the received signal strength indicator (RSSI) to the Serial Monitor
void WiFiSimple::printRSSI() {
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.print(" dBm");
}

// Print the WiFi status
void WiFiSimple::printWiFiStatus() {
  Serial.print("WiFi Status: ");
  switch(WiFi.status()) {
    case WL_CONNECTED:
      Serial.print("Connected"); break;
    case WL_AP_CONNECTED:
      Serial.print("Connected in access point mode"); break;
    case WL_AP_LISTENING :
      Serial.print("Listening for connections in access point mode"); break;
    case WL_NO_MODULE:
      Serial.print("No WiFi shield or module is present"); break;
    case WL_IDLE_STATUS:
      Serial.print("Idle"); break;
    case WL_NO_SSID_AVAIL:
      Serial.print("No SSID available"); break;
    case WL_SCAN_COMPLETED:
      Serial.print("Network scan completed"); break;
    case WL_CONNECT_FAILED:
      Serial.print("Connection failed"); break;
    case WL_CONNECTION_LOST:
      Serial.print("Connection lost"); break;
    case WL_DISCONNECTED:
      Serial.print("Disconnected from network"); break;
    default:
      Serial.print("Unknown");
  }
}

// Returns the MAC address as a string without colons
char* WiFiSimple::getMacAddrString() {
  byte mac[6];
  WiFi.macAddress(mac);
  for (int i=0; i<6; i++) {
    sprintf(&(mac_address_str_[2*i]), "%02X", mac[5-i]);
  }
  return mac_address_str_;
}