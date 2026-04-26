/*================================================================================
  IoT COUNTER WITH SERVER UPDATE
  ESP8266 counts people using IR sensor and sends data to web server via HTTP
  Features: Real-time counting, WiFi synchronization, database logging
================================================================================*/

// ===== LIBRARY INCLUDES =====
#include <ESP8266WiFi.h>                  // WiFi library for ESP8266 network connectivity
#include <ESP8266HTTPClient.h>            // HTTP client for sending data to web server

// ===== WiFi CREDENTIALS =====
const char* ssid = "YOUR_WIFI";           // Replace with your WiFi network name (SSID)
const char* password = "";                // Replace with your WiFi password

// ===== SENSOR PIN DEFINITION =====
int irSensor = D2;                        // IR sensor on GPIO4 (D2) - detects motion/objects

// ===== GLOBAL COUNTING VARIABLES =====
int count = 0;                            // Stores total count of detected objects/people
int lastState = 1;                        // Stores previous sensor state for edge detection

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== SENSOR PIN INITIALIZATION =====
  pinMode(irSensor, INPUT);               // Set IR sensor pin as INPUT to read digital signal
  
  // ===== WiFi CONNECTION =====
  WiFi.begin(ssid, password);             // Initiate connection to WiFi network

  // ===== WAIT FOR WiFi CONNECTIVITY =====
  while (WiFi.status() != WL_CONNECTED) { // Loop until WiFi connected
    delay(500);                           // Wait 500ms before checking again
  }
  // Connection established and ready for HTTP requests
}

// ===== MAIN LOOP =====
void loop() {
  // ===== READ CURRENT SENSOR STATE =====
  int state = digitalRead(irSensor);      // Read IR sensor: HIGH = no object, LOW = object detected

  // ===== DETECT FALLING EDGE (Transition from HIGH to LOW) =====
  // This method counts each passing object once (not continuous on/off)
  if (state == 0 && lastState == 1) {     // If sensor changed from HIGH (no detect) to LOW (detect)
    count++;                              // Increment total count by 1

    // ===== SEND COUNT TO WEB SERVER =====
    if (WiFi.status() == WL_CONNECTED) {  // Only send if WiFi is connected
      HTTPClient http;                    // Create HTTP client object
      
      // ===== BUILD REQUEST URL =====
      // Format: http://server-ip/update.php?count=VALUE
      String url = "http:///update.php?count=" + String(count); // Append current count to URL

      // ===== SEND HTTP GET REQUEST =====
      http.begin(url);                    // Initialize HTTP request with URL
      http.GET();                         // Send GET request to server
      http.end();                         // Close HTTP connection
    }

    delay(500);                           // Debounce delay - prevents counting same object twice
  }

  // ===== UPDATE PREVIOUS STATE =====
  lastState = state;                      // Store current state for next loop iteration
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: IoT Counter with Server Update
  DESCRIPTION: ESP8266 counts objects/people using IR sensor and sends count
               data to web server via HTTP GET request for database logging
               and real-time monitoring.
  
  COMPONENTS USED:
  - ESP8266 WiFi Module (NodeMCU or similar)
  - IR Motion/Presence Sensor (digital: HIGH/LOW)
  - Web Server (Apache/Nginx with PHP)
  
  PIN CONFIGURATION:
  - D2 (GPIO4): IR sensor digital input
  
  SENSOR SPECIFICATIONS:
  - Type: Digital IR sensor (HIGH = no object, LOW = object detected)
  - Range: 3-7 meters
  - Response time: 1-2 seconds
  
  COUNTING LOGIC:
  - Edge detection method: Counts on falling edge only
  - Falling edge: HIGH to LOW transition (object passes sensor)
  - Debounce delay: 500ms prevents multiple counts of same object
  - Total count: Continuously incremented
  
  WEB SERVER REQUIREMENTS:
  - PHP support
  - File: /update.php
  - Accepts: GET parameter 'count'
  - Example URL: http://192.168.x.x/update.php?count=123
  
  DATA TRANSMISSION:
  - Method: HTTP GET request
  - Trigger: When object detected and WiFi connected
  - Payload: Single parameter (count value)
  - Frequency: Sent after each count (debounce 500ms)
  
  FEATURES:
  - Real-time counting with WiFi synchronization
  - Automatic server database updates
  - Edge-triggered counting (no double-counting)
  - Fallback if WiFi disconnected (counts locally)
  - Serial output optional (add Serial.begin(9600) to setup)
  
  USAGE:
  1. Configure WiFi SSID and password
  2. Update server IP and PHP file path
  3. Upload sketch to ESP8266
  4. Objects passing sensor trigger count and server update
  5. Server logs count in database
  
  IMPROVEMENTS:
  - Add LCD display for real-time count
  - Add MQTT for better IoT protocol
  - Implement HTTPS for secure transmission
  - Store data locally if server unavailable
  - Add reset button
  - Add timestamp to each update
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/
