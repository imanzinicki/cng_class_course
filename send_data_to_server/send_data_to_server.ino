/*================================================================================
  IoT SENSOR DATA TO WEB SERVER
  ESP8266 sends IR and gas sensor data to web server via HTTP GET request
  Features: Dual sensor monitoring, WiFi connectivity, database logging
================================================================================*/

// ===== LIBRARY INCLUDES =====
#include <ESP8266WiFi.h>                  // WiFi library for ESP8266 network
#include <ESP8266HTTPClient.h>            // HTTP client for server communication

// ===== WiFi CREDENTIALS =====
const char* ssid = "nicki";                // WiFi network name (SSID)
const char* password = "12345678";        // WiFi network password

// ===== HTTP CLIENT OBJECT =====
WiFiClient client;                        // Create WiFi client for HTTP connections

// ===== SENSOR PIN DEFINITIONS =====
int irPin = D1;                           // IR sensor on GPIO5 (D1) - digital input
int gasPin = A0;                          // Gas sensor on A0 (analog input 0-1023)

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== SERIAL COMMUNICATION =====
  Serial.begin(115200);                   // Initialize serial at 115200 baud
  delay(1000);                            // Wait 1 second for serial startup

  // ===== DISPLAY STARTUP MESSAGE =====
  Serial.println("\nStarting System...");  // System startup notification

  // ===== SENSOR PIN INITIALIZATION =====
  pinMode(irPin, INPUT);                  // Set IR sensor as digital INPUT

  // ===== WiFi CONNECTION =====
  Serial.print("Connecting to WiFi: ");   // Status message
  Serial.println(ssid);                   // Print network name

  WiFi.begin(ssid, password);             // Start WiFi connection

  // ===== WAIT FOR WiFi CONNECTION =====
  while (WiFi.status() != WL_CONNECTED) { // Loop until connected
    delay(500);                           // Wait 500ms between checks
    Serial.print(".");                    // Print dot for each attempt
  }

  // ===== DISPLAY CONNECTION SUCCESS =====
  Serial.println("\nWiFi Connected!");    // Connection successful
  Serial.print("ESP IP Address: ");       // IP address label
  Serial.println(WiFi.localIP());         // Print assigned IP address
}

// ===== MAIN LOOP =====
void loop() {

  // ===== CHECK WiFi CONNECTION =====
  if (WiFi.status() == WL_CONNECTED) {   // Only proceed if WiFi connected

    // ===== READ SENSORS =====
    int ir = digitalRead(irPin);          // Read IR sensor (HIGH or LOW)
    int gas = analogRead(gasPin);         // Read gas sensor (0-1023)

    // ===== DISPLAY SENSOR VALUES =====
    Serial.println("\n--- SENSOR DATA ---"); // Header for sensor readings
    Serial.print("IR Value: ");           // IR sensor label
    Serial.println(ir);                   // Print IR value

    Serial.print("Gas Value: ");          // Gas sensor label
    Serial.println(gas);                  // Print gas value (0-1023)

    // ===== CREATE HTTP CLIENT =====
    HTTPClient http;                      // Create HTTP client object

    // ===== BUILD REQUEST URL =====
    // Format: http://server-ip/iot/receive.php?ir=VALUE&gas=VALUE
    String url = "http://192.168.1.106/iot/receive.php?ir=" + String(ir) + "&gas=" + String(gas);

    // ===== DISPLAY REQUEST URL =====
    Serial.println("Sending request...");  // Status message
    Serial.println(url);                   // Print full URL being requested

    // ===== INITIALIZE HTTP REQUEST =====
    http.begin(client, url);              // Initialize HTTP request with WiFi client and URL

    // ===== SEND HTTP GET REQUEST =====
    int httpCode = http.GET();             // Send GET request, receive HTTP response code
    
    // ===== READ SERVER RESPONSE BODY =====
    String payload = http.getString();     // Read response data from server

    // ===== DISPLAY RESPONSE CODE =====
    Serial.print("HTTP Code: ");          // Response code label
    Serial.println(httpCode);              // Print HTTP status code (200=success, 404=not found, etc)

    // ===== DISPLAY SERVER RESPONSE =====
    Serial.print("Server Response: ");    // Response label
    Serial.println(payload);               // Print response content (usually HTML or JSON)

    // ===== EVALUATE REQUEST SUCCESS =====
    if (httpCode == 200) {                // If HTTP 200 (OK), request was successful
      Serial.println("Data sent successfully!"); // Success message
    } else {
      Serial.println("Failed to send data!"); // Failure message
    }

    // ===== CLOSE HTTP CONNECTION =====
    http.end();                           // Close HTTP connection, free memory

  } else {
    // ===== WiFi DISCONNECTED =====
    Serial.println("WiFi Disconnected! Reconnecting..."); // WiFi lost message
  }

  delay(5000);                            // Wait 5 seconds before next data send
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: IoT Sensor Data to Web Server
  DESCRIPTION: ESP8266 reads dual sensors (IR and gas) and sends data to
               remote web server via HTTP GET request. Data is typically
               logged in database on server for analysis and visualization.
  
  COMPONENTS USED:
  - ESP8266 WiFi Module (NodeMCU or similar)
  - IR Motion/Presence Sensor (digital: HIGH/LOW)
  - MQ-Series Gas Sensor (analog: 0-1023)
  - Web Server (Apache/Nginx with PHP)
  
  PIN CONFIGURATION:
  - D1 (GPIO5): IR sensor digital input
  - A0 (ADC0): Gas sensor analog input
  
  SENSOR SPECIFICATIONS:
  IR Sensor:
  - Type: Digital (HIGH = detected, LOW = not detected)
  - Range: 3-7 meters
  - Response time: 1-2 seconds
  
  Gas Sensor (MQ-x series):
  - Type: Analog (0-1023 where 1023 = high concentration)
  - Detectable gases: CO, LPG, methane, alcohol, smoke
  - Warm-up time: 24 hours for accuracy
  - Useful range: 200-800 (below 200 = background)
  
  WEB SERVER REQUIREMENTS:
  - PHP support
  - File: /iot/receive.php
  - Accepts: GET parameters ir and gas
  - Example URL: http://192.168.1.106/iot/receive.php?ir=1&gas=456
  
  DATA TRANSMISSION:
  - Method: HTTP GET request
  - Frequency: Every 5 seconds
  - Payload: Two sensor values (ir, gas)
  - Response: Server HTML/JSON response
  
  EXAMPLE SERVER RESPONSE (PHP):
  ============================================
  <?php
    $ir = $_GET['ir'];
    $gas = $_GET['gas'];
    // Save to database
    $sql = "INSERT INTO sensors (ir, gas) VALUES ($ir, $gas)";
    // Return success
    echo "Data received: IR=$ir, Gas=$gas";
  ?>
  ============================================
  
  TYPICAL DATA SEQUENCE:
  1. Read IR sensor (0 or 1)
  2. Read gas sensor (0-1023)
  3. Build URL with sensor values
  4. Send HTTP GET request
  5. Receive server response
  6. Wait 5 seconds
  7. Repeat
  
  MONITORING DASHBOARD:
  Create web interface to:
  - Display real-time sensor values
  - Show historical graphs
  - Set alert thresholds
  - Export data to CSV/Excel
  
  TROUBLESHOOTING:
  - No WiFi connection: Check SSID/password
  - HTTP 404: Server file missing (check receive.php path)
  - HTTP 500: Server PHP error (check syntax)
  - No sensor data: Check pin connections
  - Fluctuating gas values: Warm up sensor 24 hours
  
  IMPROVEMENTS:
  - Add MQTT for better IoT protocol
  - Implement HTTPS for secure transmission
  - Add sensor calibration routine
  - Store data locally if server unavailable
  - Add timestamp to each reading
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/