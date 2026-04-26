/*================================================================================
  ESP8266 WiFi CONNECTION TEST
  Establishes connection to WiFi network and displays IP address
  Used for network diagnostics and connectivity verification
================================================================================*/

// ===== LIBRARY INCLUDES =====
#include <ESP8266WiFi.h>                  // WiFi library for ESP8266 module

// ===== WiFi CREDENTIALS =====
char* ssid = "nicki";                     // WiFi network name (SSID)
const char* password = "12345678";        // WiFi network password

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== SERIAL COMMUNICATION =====
  Serial.begin(115200);                   // Initialize serial at 115200 baud (ESP8266 default)
  delay(10);                              // Wait 10ms for serial to stabilize

  // ===== DISPLAY CONNECTION MESSAGE =====
  Serial.println();                       // Print empty line for readability
  Serial.println("Connecting to Wi-Fi..."); // Display connection status message

  // ===== INITIATE WiFi CONNECTION =====
  WiFi.begin(ssid, password);             // Start WiFi connection using SSID and password
  
  // ===== WAIT FOR CONNECTION =====
  while (WiFi.status() != WL_CONNECTED) { // Loop until WiFi connection is established
    Serial.print(".");                    // Print dot for each connection attempt
    // Connection timeout handled by ESP8266 internally (typically ~20 seconds)
  }

  // ===== DISPLAY CONNECTION SUCCESS =====
  Serial.println();                       // Print empty line for readability
  Serial.println("Wi-Fi connected!");     // Display success message
  
  // ===== DISPLAY ASSIGNED IP ADDRESS =====
  Serial.print("IP Address: ");           // Print label for IP address
  Serial.println(WiFi.localIP());         // Print the IP address assigned by DHCP server
}

// ===== MAIN LOOP =====
void loop() {
  // Note: This sketch only tests connection. Implement your main code here.
  // Connection is maintained automatically by ESP8266 WiFi stack.
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: ESP8266 WiFi Connection Test
  DESCRIPTION: Simple sketch to verify ESP8266 WiFi connectivity and obtain
               DHCP-assigned IP address. Used for network diagnostics.
  
  COMPONENTS USED:
  - ESP8266 WiFi Module (NodeMCU or similar)
  - USB Serial connection for monitoring
  
  USAGE:
  1. Update SSID and password for your WiFi network
  2. Upload to ESP8266 via Arduino IDE
  3. Open Serial Monitor at 115200 baud
  4. Observe connection dots and final IP address
  
  EXPECTED OUTPUT:
  "Connecting to Wi-Fi..."
  "..................." (connection attempts)
  "Wi-Fi connected!"
  "IP Address: 192.168.x.x" (your assigned IP)
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/