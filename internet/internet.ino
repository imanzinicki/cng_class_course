/*================================================================================
  ESP8266 DUAL WiFi MODE (Station + Access Point)
  Connects to home WiFi while simultaneously running as WiFi hotspot
  Features: Home network + local hotspot in one device
================================================================================*/

// ===== LIBRARY INCLUDES =====
#include <ESP8266WiFi.h>                  // WiFi library for ESP8266 dual mode

// ===== STATION MODE CREDENTIALS (Connection to Home WiFi) =====
const char* sta_ssid = "YOUR_HOME_WIFI";   // Your home WiFi network name (SSID)
const char* sta_password = "YOUR_HOME_PASSWORD"; // Your home WiFi password

// ===== ACCESS POINT MODE CREDENTIALS (Hotspot) =====
const char* ap_ssid = "ESP_Hotspot";       // Name of hotspot this device broadcasts
const char* ap_password = "12345678";      // Hotspot access password

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== SERIAL COMMUNICATION =====
  Serial.begin(115200);                   // Initialize serial at 115200 baud (ESP8266 standard)
  

  // ===== ENABLE DUAL WiFi MODE (STA + AP) =====
  // STA = Station mode (connects to existing network)
  // AP = Access Point mode (broadcasts its own network)
  WiFi.mode(WIFI_AP_STA);                 // Enable both station and AP modes simultaneously

  // ===== STATION MODE: Connect to Home WiFi =====
  WiFi.begin(sta_ssid, sta_password);     // Start connection to home WiFi network
  Serial.println("Connecting to Wi-Fi..."); // Status message
  
  // ===== WAIT FOR STATION CONNECTION =====
  while (WiFi.status() != WL_CONNECTED) { // Loop until connected to home network
    delay(500);                           // Check every 500ms
    Serial.print(".");                    // Print dot for each attempt
  }
  
  // ===== DISPLAY STATION CONNECTION SUCCESS =====
  Serial.println();                       // New line for readability
  Serial.println("Connected to Wi-Fi!");  // Connection successful
  Serial.print("ESP IP on your network: "); // Label for station IP
  Serial.println(WiFi.localIP());         // Print IP assigned by home network DHCP

  // ===== ACCESS POINT MODE: Create Local Hotspot =====
  // This runs in parallel with station mode
  WiFi.softAP(ap_ssid, ap_password);      // Start hotspot with SSID and password
  Serial.println("Hotspot started!");     // Status message
  Serial.print("Hotspot IP: ");           // Label for access point IP
  Serial.println(WiFi.softAPIP());        // Print AP IP (usually 192.168.4.1)
}

// ===== MAIN LOOP =====
void loop() {
  // Both WiFi modes run automatically in background
  // Add your application code here
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: ESP8266 Dual WiFi Mode (Station + Access Point)
  DESCRIPTION: ESP8266 connects to home WiFi network while simultaneously
               broadcasting its own WiFi hotspot. Enables both internet access
               and local device communication.
  
  COMPONENTS USED:
  - ESP8266 WiFi Module (NodeMCU, D1 Mini, or similar)
  
  OPERATION MODES:
  Station Mode (STA):
  - Connects to existing home WiFi network
  - Gets IP from home router DHCP
  - Can access internet and home devices
  - IP: 192.168.x.x (assigned by router)
  
  Access Point Mode (AP):
  - Broadcasts own WiFi network
  - Other devices connect to "ESP_Hotspot"
  - All connections are local to ESP8266
  - IP: 192.168.4.1 (default)
  
  NETWORK DIAGRAM:
     [Home WiFi Router]
              |
         [ESP8266] <-- Station mode
          /     \
        /         \  
    [Devices]   [Access Point]
    (via home)    (via hotspot)
  
  USAGE:
  1. Update sta_ssid and sta_password for home network
  2. Upload sketch to ESP8266
  3. Open Serial Monitor at 115200 baud
  4. Observe connection messages
  5. Two IPs displayed:
     - Station IP: Assigned by home router
     - Hotspot IP: 192.168.4.1
  
  DEVICE CONNECTIONS:
  A. Via Home Network:
     - Find your ESP8266's station IP
     - Other home devices can access it
  
  B. Via Local Hotspot:
     - Look for "ESP_Hotspot" WiFi network
     - Connect with password "12345678"
     - Access ESP8266 at 192.168.4.1
  
  ADVANTAGES:
  - Single device provides internet and local access
  - Useful for IoT devices without home WiFi
  - Local hotspot doesn't require internet
  - Can bridge home network to isolated devices
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/