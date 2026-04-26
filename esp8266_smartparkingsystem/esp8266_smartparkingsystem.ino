/*================================================================================
  IoT SMART PARKING SYSTEM
  ESP8266 with Blynk cloud for automated parking management
  Features: Entrance detection, slot monitoring, gate control, Blynk reporting
================================================================================*/

// ===== BLYNK AUTHENTICATION =====
#define BLYNK_TEMPLATE_ID "TMPL2Rjf-9Z0O"        // Blynk template ID
#define BLYNK_TEMPLATE_NAME "IOT SMART PARKING SYSTEM" // Project name

// ===== LIBRARY INCLUDES =====
#include <ESP8266WiFi.h>                  // WiFi library for ESP8266
#include <BlynkSimpleEsp8266.h>           // Blynk IoT integration
#include <Servo.h>                        // Servo motor control for gate

// ===== SENSOR PIN DEFINITIONS =====
#define ENTRANCE_SENSOR D1                // IR sensor at entrance (D1/GPIO5) detects approaching car
#define SLOT1_SENSOR    D2                // Parking slot 1 occupancy sensor (D2/GPIO4)
#define SLOT2_SENSOR    D3                // Parking slot 2 occupancy sensor (D3/GPIO0)
#define SLOT3_SENSOR    D4                // Parking slot 3 occupancy sensor (D4/GPIO2)

// ===== ACTUATOR PIN DEFINITIONS =====
#define SERVO_PIN  D5                     // Servo motor for gate control (D5/GPIO14)
#define BUZZER_PIN D6                     // Buzzer for alerts (D6/GPIO12)
#define LED_RED    D7                     // Red LED for full parking indicator (D7/GPIO13)
#define LED_GREEN  D8                     // Green LED for available slots (D8/GPIO15)

// ===== GATE SERVO POSITIONS =====
#define GATE_OPEN 90                      // Servo angle when gate is open
#define GATE_CLOSED 0                     // Servo angle when gate is closed

// ===== SERVO OBJECT =====
Servo gate;                               // Servo object for automated gate control

// ===== WiFi CREDENTIALS =====
char ssid[] = "1CNG THECADEMIE LTD";       // WiFi network name
char pass[] = "cng12345678";               // WiFi password

// ===== FUNCTION: CHECK SENSOR STATE =====
// Returns true if sensor detects object (LOW), false if clear (HIGH)
bool isDetected(int pin) {
  return digitalRead(pin) == LOW;         // LOW = detected (object/car present), HIGH = clear
}

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== SERIAL COMMUNICATION =====
  Serial.begin(115200);                   // Initialize serial at 115200 baud (ESP8266 standard)

  // ===== SENSOR PIN INITIALIZATION =====
  pinMode(ENTRANCE_SENSOR, INPUT);        // Entrance sensor as INPUT
  pinMode(SLOT1_SENSOR, INPUT);           // Slot 1 sensor as INPUT
  pinMode(SLOT2_SENSOR, INPUT);           // Slot 2 sensor as INPUT
  pinMode(SLOT3_SENSOR, INPUT);           // Slot 3 sensor as INPUT

  // ===== ACTUATOR PIN INITIALIZATION =====
  pinMode(BUZZER_PIN, OUTPUT);            // Buzzer as OUTPUT
  pinMode(LED_RED, OUTPUT);               // Red LED as OUTPUT
  pinMode(LED_GREEN, OUTPUT);             // Green LED as OUTPUT

  // ===== SERVO INITIALIZATION =====
  gate.attach(SERVO_PIN);                 // Attach servo to GPIO pin
  gate.write(GATE_CLOSED);                // Set initial gate position to closed

  // ===== BLYNK CONNECTION =====
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Connect to Blynk cloud
}

// ===== FUNCTION: BUZZER ALERT FOR PARKING FULL =====
void buzzFull() {
  digitalWrite(BUZZER_PIN, HIGH);         // Turn buzzer ON
  delay(150);                             // Buzzer active for 150ms
  digitalWrite(BUZZER_PIN, LOW);          // Turn buzzer OFF
}

// ===== MAIN LOOP =====
void loop() {
  // ===== MAINTAIN BLYNK CONNECTION =====
  Blynk.run();                            // Process Blynk requests

  // ===== READ ALL SENSORS =====
  bool entrance = isDetected(ENTRANCE_SENSOR); // TRUE = car detected at entrance
  bool slot1 = isDetected(SLOT1_SENSOR);       // TRUE = slot 1 occupied
  bool slot2 = isDetected(SLOT2_SENSOR);       // TRUE = slot 2 occupied
  bool slot3 = isDetected(SLOT3_SENSOR);       // TRUE = slot 3 occupied

  // ===== CALCULATE PARKING STATUS =====
  int occupied = slot1 + slot2 + slot3;   // Count total occupied slots (0-3)
  int freeSlots = 3 - occupied;           // Calculate available slots

  // ===== SEND DATA TO BLYNK APP =====
  Blynk.virtualWrite(V0, entrance ? 0:255); // V0: Entrance indicator (LED gauge)
  Blynk.virtualWrite(V1, slot1 ? 255 : 0);  // V1: Slot 1 status (red = occupied)
  Blynk.virtualWrite(V2, slot2 ? 255 : 0);  // V2: Slot 2 status
  Blynk.virtualWrite(V3, slot3 ? 255 : 0);  // V3: Slot 3 status
  Blynk.virtualWrite(V4, freeSlots);        // V4: Number of free slots (display)

  // ===== SEND PARKING STATUS MESSAGE =====
  if (freeSlots == 0) {
    Blynk.virtualWrite(V5, "PARKING FULL");  // V5: Status text when parking full
  } else {
    Blynk.virtualWrite(V5, "SLOTS AVAILABLE"); // V5: Status when slots are available
  } 

  // ===== CONTROL STATUS LEDS =====
  digitalWrite(LED_GREEN, freeSlots > 0); // Green LED ON if slots available
  digitalWrite(LED_RED, freeSlots == 0);  // Red LED ON if parking full

  // ===== GATE CONTROL LOGIC =====
  if (freeSlots == 0) {                   // If parking is FULL
    gate.write(GATE_CLOSED);              // Keep gate CLOSED
    Blynk.virtualWrite(V6, "CLOSED (FULL)"); // V6: Gate status message
  } else if (entrance) {                  // Else if car detected at entrance AND slots available
    gate.write(GATE_OPEN);                // Open gate for entry
    Blynk.virtualWrite(V6, "OPEN");       // Report gate open
  } else {                                // Else no car at entrance
    gate.write(GATE_CLOSED);              // Close gate
    Blynk.virtualWrite(V6, "CLOSED");     // Report gate closed
  }

  // ===== ALERT IF PARKING FULL =====
  if (freeSlots == 0) {                   // If all slots occupied
    buzzFull();                           // Sound buzzer alert
  }

  delay(100);                             // Update every 100ms
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: IoT Smart Parking System
  DESCRIPTION: Automated parking management system that monitors slot occupancy
               and controls entrance gate. Real-time status updates via Blynk
               smartphone app. Prevents parking when lot is full.
  
  COMPONENTS USED:
  - ESP8266 WiFi Module (NodeMCU or similar)
  - 4x IR Parking Sensors (entrance + 3 slots)
  - SG90 Servo Motor (gate mechanism)
  - Active Buzzer (parking full alert)
  - 2x LED Indicators (red/green status)
  - Blynk IoT Cloud Platform
  
  PIN CONFIGURATION:
  - D1 (GPIO5): Entrance sensor input
  - D2 (GPIO4): Slot 1 sensor input
  - D3 (GPIO0): Slot 2 sensor input
  - D4 (GPIO2): Slot 3 sensor input
  - D5 (GPIO14): Servo motor PWM output
  - D6 (GPIO12): Buzzer output
  - D7 (GPIO13): Red LED output (parking full)
  - D8 (GPIO15): Green LED output (slots available)
  
  BLYNK VIRTUAL PINS:
  - V0: Entrance detection (LED gauge)
  - V1: Slot 1 status (red = occupied)
  - V2: Slot 2 status
  - V3: Slot 3 status
  - V4: Number of available slots (number display)
  - V5: Parking status message (text)
  - V6: Gate status (text: OPEN, CLOSED, CLOSED FULL)
  
  LOGIC:
  1. Monitor entrance and all 3 parking slots
  2. Calculate available slots = 3 - occupied
  3. If slots available: OPEN gate when car detected
  4. If all full: CLOSE gate and sound buzzer
  5. Update Blynk app in real-time
  
  FEATURES:
  - Real-time slot occupancy monitoring
  - Automatic gate control based on availability
  - Smart parking full detection
  - Mobile app integration via Blynk
  - Visual LED indicators (red/green)
  - Audio alert when parking full
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/
}