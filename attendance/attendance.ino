/*================================================================================
  FINGERPRINT ATTENDANCE SYSTEM
  Arduino with Adafruit Fingerprint Sensor for automated attendance tracking
  Features: Student enrollment, fingerprint matching, Python integration
================================================================================*/

// ===== LIBRARY INCLUDES =====
#include <Adafruit_Fingerprint.h>         // Adafruit fingerprint sensor library
#include <SoftwareSerial.h>               // Software serial for sensor communication

// ===== SOFTWARE SERIAL FOR FINGERPRINT SENSOR =====
// Creates virtual serial port on pins 10 (RX) and 11 (TX) for sensor communication
SoftwareSerial mySerial(10, 11);          // RX on pin 10, TX on pin 11 to sensor

// ===== FINGERPRINT SENSOR OBJECT =====
Adafruit_Fingerprint finger(&mySerial);   // Create fingerprint object using software serial

// ===== GLOBAL VARIABLES =====
int nextID = 1;                           // Stores next available fingerprint ID (starts at 1)
bool enrolling = false;                   // Flag to track if system is in enrollment mode
String enrollName = "";                   // Stores student name during enrollment (unused but reserved)

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== SERIAL COMMUNICATION =====
  Serial.begin(9600);                     // Initialize main serial for Python communication (9600 baud)
  
  // ===== FINGERPRINT SENSOR INITIALIZATION =====
  mySerial.begin(57600);                  // Initialize sensor serial communication (57600 baud - sensor requirement)

  // ===== VERIFY SENSOR CONNECTION =====
  if (finger.verifyPassword()) {          // Check if sensor responds with correct password
    Serial.println("Sensor found ✅");    // Sensor connected successfully
  } else {
    Serial.println("Sensor not found ❌"); // Sensor not responding
    while (1);                            // Halt execution indefinitely if sensor fails
  }
}

// ===== MAIN LOOP =====
void loop() {
  // ===== LISTEN FOR PYTHON COMMANDS =====
  if (Serial.available()) {               // Check if data received from Python/Serial
    String cmd = Serial.readStringUntil('\n'); // Read command until newline character
    cmd.trim();                           // Remove leading/trailing whitespace
    
    if (cmd == "ENROLL") {                // If command is "ENROLL", enter enrollment mode
      enrolling = true;                   // Set enrollment flag to true
    }
  }

  // ===== ENROLLMENT PROCESS =====
  if (enrolling) {                        // If system is in enrollment mode
    enrollFinger(nextID);                 // Enroll fingerprint with current ID
    Serial.print("ENROLLED:");            // Send enrollment confirmation
    Serial.println(nextID);               // Print the enrolled ID number
    nextID++;                             // Increment ID for next enrollment
    enrolling = false;                    // Exit enrollment mode
  }

  // ===== FINGERPRINT MATCHING / ATTENDANCE TRACKING =====
  uint8_t p = finger.getImage();          // Capture fingerprint image from sensor
  if (p == FINGERPRINT_NOFINGER) return;  // If no finger detected, exit early
  else if (p != FINGERPRINT_OK) return;   // If image capture failed, exit early

  p = finger.image2Tz();                  // Convert captured image to fingerprint template
  if (p != FINGERPRINT_OK) return;        // If conversion failed, exit early

  p = finger.fingerFastSearch();          // Search fingerprint database for match
  if (p == FINGERPRINT_OK) {              // If match found
    Serial.print("ID:");                  // Send matched ID prefix
    Serial.println(finger.fingerID);      // Print the matched student ID (Python reads this)
  }
}

// ===== FUNCTION: ENROLL NEW FINGERPRINT =====
// Captures fingerprint twice and stores template in sensor database
void enrollFinger(int id) {               // id = unique enrollment ID number
  int p = -1;                             // Variable to store sensor response status
  
  // ===== FIRST FINGERPRINT CAPTURE =====
  Serial.println("Place finger for enrollment..."); // Prompt user to place finger
  while (p != FINGERPRINT_OK) {           // Loop until valid image is captured
    p = finger.getImage();                // Attempt to capture fingerprint image
  }

  // ===== CONVERT FIRST IMAGE TO TEMPLATE =====
  p = finger.image2Tz(1);                 // Convert image to template, store in slot 1
  if (p != FINGERPRINT_OK) return;        // If conversion failed, exit function

  // ===== PROMPT USER TO REMOVE FINGER =====
  Serial.println("Remove finger...");     // Tell user to lift finger off sensor
  delay(2000);                            // Wait 2 seconds for finger removal
  while (finger.getImage() != FINGERPRINT_NOFINGER); // Wait until finger is removed

  // ===== SECOND FINGERPRINT CAPTURE (Verification) =====
  Serial.println("Place same finger again..."); // Prompt for second scan of same finger
  p = -1;                                 // Reset status variable
  while (p != FINGERPRINT_OK) {           // Loop until valid second image is captured
    p = finger.getImage();                // Attempt second fingerprint capture
  }

  // ===== CONVERT SECOND IMAGE TO TEMPLATE =====
  p = finger.image2Tz(2);                 // Convert second image to template, store in slot 2
  if (p != FINGERPRINT_OK) return;        // If conversion failed, exit function

  // ===== CREATE FINGERPRINT MODEL FROM TWO TEMPLATES =====
  p = finger.createModel();               // Combine both templates to create final model
  if (p != FINGERPRINT_OK) return;        // If model creation failed, exit function

  // ===== STORE MODEL IN SENSOR DATABASE =====
  finger.storeModel(id);                  // Save fingerprint model with assigned ID number
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: Fingerprint Attendance System
  DESCRIPTION: Arduino-based attendance system that enrolls student fingerprints
               and marks attendance when fingerprints are scanned. Communicates
               with Python backend for data management.
  
  COMPONENTS USED:
  - Arduino UNO Microcontroller
  - Adafruit R307 Fingerprint Sensor (57600 baud)
  - USB Serial connection to Python application
  
  PIN CONFIGURATION:
  - Pin 10 (RX): Fingerprint sensor RX data line
  - Pin 11 (TX): Fingerprint sensor TX data line
  - USB Serial: Python communication (9600 baud)
  
  PROTOCOL:
  Python sends: "ENROLL" → Arduino prompts fingerprint capture → Arduino sends "ENROLLED:ID"
  During scanning: Fingerprint match → Arduino sends "ID:StudentID" → Python logs attendance
  
  FEATURES:
  - Dual fingerprint capture for accuracy
  - Unique student ID assignment
  - Real-time Python integration
  - Error handling for sensor failures
  
  USAGE:
  1. Run Python attendance script
  2. Type "ENROLL" when prompted
  3. Place finger on sensor (two scans required)
  4. System saves fingerprint with assigned ID
  5. Subsequent scans automatically mark attendance
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/