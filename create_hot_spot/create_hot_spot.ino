/*================================================================================
  ESP8266 WiFi HOTSPOT SERVER
  Creates a WiFi access point (hotspot) for direct device connection
  Useful for local device communication without external WiFi network
================================================================================*/

// ===== LIBRARY INCLUDES =====
#include <ESP8266WiFi.h>                  // WiFi library for ESP8266 network functionality

// ===== HOTSPOT CREDENTIALS =====
char* ssid = "ESP_Hotspot";               // Access point name (what users see when scanning)
char* password = "12345678";              // Access point password for connection

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== SERIAL COMMUNICATION =====
  Serial.begin(115200);                   // Initialize serial at 115200 baud (ESP8266 standard)
  Serial.println();                       // Print newline for readability

  // ===== CREATE WIFI HOTSPOT (Soft Access Point) =====
  // This creates a WiFi network that devices can connect to
  WiFi.softAP(ssid, password);            // Create AP with SSID and password

  // ===== DISPLAY HOTSPOT STATUS =====
  Serial.println("Hotspot started!");     // Confirm hotspot is active
  Serial.print("IP Address: ");           // Display label for access point IP
  Serial.println(WiFi.softAPIP());        // Print the IP address (usually 192.168.4.1)
}

// ===== MAIN LOOP =====
void loop() {
  // Hotspot runs automatically in background
  // Add your main application code here
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: ESP8266 WiFi Hotspot Server
  DESCRIPTION: Creates a WiFi access point allowing other devices to connect
               directly to the ESP8266 without requiring an external network.
               Useful for IoT devices in areas without WiFi infrastructure.
  
  COMPONENTS USED:
  - ESP8266 WiFi Module (NodeMCU, D1 Mini, or similar)
  
  FEATURES:
  - Operates as WiFi access point (soft AP mode)
  - Devices connect using SSID and password
  - Default gateway IP: 192.168.4.1
  - Supports up to 8 simultaneous connections
  
  USAGE:
  1. Upload sketch to ESP8266
  2. Look for WiFi network named "ESP_Hotspot"
  3. Connect with password "12345678"
  4. Access ESP8266 at IP 192.168.4.1
  5. Implement web server or data exchange in loop()
  
  SECURITY NOTE:
  - Change default password immediately in production
  - Consider WPA2 encryption for sensitive applications
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/