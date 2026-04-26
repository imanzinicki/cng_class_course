/*================================================================================
  FINGERPRINT SENSOR BASIC TEST
  Simple fingerprint matching test without enrollment
  Displays matched fingerprint ID when finger is scanned
================================================================================*/

// ===== LIBRARY INCLUDES =====
#include <Adafruit_Fingerprint.h>         // Adafruit fingerprint sensor library
#include <SoftwareSerial.h>               // Software serial for sensor communication

// ===== SOFTWARE SERIAL FOR FINGERPRINT SENSOR =====
// Pins 4 (RX) and 5 (TX) for sensor communication
SoftwareSerial mySerial(4, 5);            // RX on pin 4, TX on pin 5

// ===== FINGERPRINT SENSOR OBJECT =====
Adafruit_Fingerprint finger(&mySerial);   // Create fingerprint sensor instance

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== SERIAL COMMUNICATION =====
  Serial.begin(9600);                     // Initialize main serial (9600 baud)
  
  // ===== WAIT FOR SERIAL CONNECTION =====
  while (!Serial);                        // Wait for Serial connection to establish
  
  // ===== DISPLAY TEST MESSAGE =====
  Serial.println("Fingerprint sensor test"); // Print test start message

  // ===== FINGERPRINT SENSOR INITIALIZATION =====
  finger.begin(57600);                    // Initialize sensor serial (57600 baud - standard)
  
  // ===== VERIFY SENSOR CONNECTION =====
  if (finger.verifyPassword()) {          // Check if sensor responds
    Serial.println("Sensor found!");      // Sensor connected successfully
  } else {
    Serial.println("Sensor not found :("); // Sensor not responding
    while (1) { delay(1); }               // Halt execution
  }
}

// ===== MAIN LOOP =====
void loop() {
  // ===== PROMPT USER TO SCAN FINGERPRINT =====
  Serial.println("\nPlace your finger..."); // Ask user to place finger on sensor
  
  // ===== GET FINGERPRINT ID =====
  int id = getFingerprintID();            // Call function to capture and identify fingerprint
  
  // ===== DISPLAY RESULT IF MATCH FOUND =====
  if (id != -1) {                         // If ID is valid (not error code -1)
    Serial.print("Finger ID: ");          // Print label
    Serial.println(id);                   // Print the matched ID number
  }
  
  delay(2000);                            // Wait 2 seconds before next scan prompt
}

// ===== FUNCTION: GET FINGERPRINT ID =====
// Scans fingerprint and searches database for match
// Returns: Matched ID (1-127) or -1 if no match found
int getFingerprintID() {
  // ===== CAPTURE FINGERPRINT IMAGE =====
  int p = finger.getImage();              // Attempt to capture fingerprint image
  if (p != FINGERPRINT_OK) return -1;     // If capture failed, return error code

  // ===== CONVERT IMAGE TO TEMPLATE =====
  p = finger.image2Tz();                  // Convert captured image to searchable template
  if (p != FINGERPRINT_OK) return -1;     // If conversion failed, return error code

  // ===== SEARCH DATABASE FOR MATCH =====
  p = finger.fingerFastSearch();          // Search stored fingerprints for a match
  if (p != FINGERPRINT_OK) return -1;     // If search failed or no match, return error code

  // ===== RETURN MATCHED ID =====
  return finger.fingerID;                 // Return the ID of matched fingerprint (1-127)
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: Fingerprint Sensor Basic Test
  DESCRIPTION: Simple fingerprint scanner test that identifies pre-enrolled
               fingerprints. Useful for testing sensor functionality and
               verifying enrolled fingerprints. Does NOT enroll new fingerprints.
  
  COMPONENTS USED:
  - Arduino UNO Microcontroller
  - Adafruit R307 Fingerprint Sensor Module (57600 baud)
  
  PIN CONFIGURATION:
  - Pin 4: Software Serial RX (sensor data input)
  - Pin 5: Software Serial TX (sensor data output)
  
  FEATURES:
  - Real-time fingerprint matching
  - Fast search algorithm (returns match within seconds)
  - Supports 127 different fingerprint IDs
  - Serial monitor output for verification
  - Error handling for sensor failures
  
  USAGE:
  1. Ensure fingerprints are already enrolled in sensor
  2. Upload sketch to Arduino
  3. Open Serial Monitor at 9600 baud
  4. Place enrolled finger on sensor
  5. System displays matched ID or error message
  6. Repeat for multiple fingers
  
  OUTPUT MESSAGES:
  - "Sensor found!": Sensor connected successfully
  - "Sensor not found": Check connections and power
  - "Place your finger...": Ready for fingerprint scan
  - "Finger ID: X": Successful match, ID is X
  - "No match": Fingerprint not in database (implied by repeated prompt)
  
  ERROR CODES:
  - -1: No match found or sensor error
  - Other codes: Internal sensor errors
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/