/*================================================================================
  FINGERPRINT ENROLLMENT SYSTEM
  Arduino with Adafruit R307 fingerprint sensor for automated student enrollment
  Features: Dual-scan enrollment, auto-incrementing student IDs, serial feedback
================================================================================*/

// ===== LIBRARY INCLUDES =====
#include <Adafruit_Fingerprint.h>         // Adafruit fingerprint sensor library
#include <SoftwareSerial.h>               // Software serial for sensor communication

// ===== SOFTWARE SERIAL FOR FINGERPRINT SENSOR =====
// Pins 2 (RX) and 3 (TX) communicate with fingerprint sensor
SoftwareSerial sensor(2, 3);              // RX on pin 2, TX on pin 3

// ===== FINGERPRINT SENSOR OBJECT =====
Adafruit_Fingerprint finger(&sensor);     // Create fingerprint object

// ===== GLOBAL VARIABLE FOR STUDENT ID =====
int studentID = 1;                        // Starting student ID (auto-increments per enrollment)

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
// ===== SERIAL COMMUNICATION =====
Serial.begin(9600);                       // Initialize main serial (9600 baud)

// ===== FINGERPRINT SENSOR INITIALIZATION =====
finger.begin(57600);                      // Initialize sensor serial (57600 baud)

// ===== VERIFY SENSOR CONNECTION =====
if (!finger.verifyPassword()) {           // Check if sensor responds correctly
Serial.println("Sensor not found");       // Sensor not responding
while (1);                                // Halt if sensor fails
}

// ===== DISPLAY READY MESSAGE =====
Serial.println("Ready to enroll students"); // System ready for enrollment
}

// ===== MAIN LOOP =====
void loop() {
// ===== PROMPT USER TO PLACE FINGER =====
Serial.println("Place finger to register"); // Instruction for student

// ===== ENROLL NEW FINGERPRINT =====
int result = enrollFinger(studentID);     // Call enrollment function with current ID

// ===== HANDLE ENROLLMENT RESULT =====
if (result == FINGERPRINT_OK) {           // If enrollment successful
Serial.print("Student saved with ID: ");  // Confirmation message
Serial.println(studentID);                // Print the enrolled ID
studentID++;                              // Increment ID for next student
} else {
Serial.println("Failed to register");     // Enrollment failed message
}

delay(5000);                              // Wait 5 seconds before next enrollment prompt
}

// ===== FUNCTION: ENROLL NEW FINGERPRINT =====
// Captures fingerprint twice and stores in sensor database
// Returns: FINGERPRINT_OK if successful, error code otherwise
int enrollFinger(int id) {                // id = unique enrollment ID number

int p = -1;                               // Variable to store sensor response

// ===== FIRST FINGERPRINT CAPTURE =====
// Capture first image of the fingerprint
while (p != FINGERPRINT_OK) {             // Loop until valid image captured
p = finger.getImage();                    // Attempt to capture fingerprint image
}

// ===== CONVERT FIRST IMAGE TO TEMPLATE =====
p = finger.image2Tz(1);                   // Convert image to template, store in slot 1
if (p != FINGERPRINT_OK) return p;        // If conversion failed, return error

// ===== PROMPT USER TO REMOVE FINGER =====
Serial.println("Remove finger");          // Tell user to lift finger off sensor
delay(2000);                              // Wait 2 seconds for finger removal

// ===== VERIFY FINGER REMOVED =====
// Wait until sensor confirms finger is no longer present
while (finger.getImage() != FINGERPRINT_NOFINGER);

// ===== SECOND FINGERPRINT CAPTURE (Verification) =====
// Capture second image of same finger for verification
Serial.println("Place same finger again"); // Ask user to place same finger again

p = -1;                                   // Reset status variable
while (p != FINGERPRINT_OK) {             // Loop until valid second image captured
p = finger.getImage();                    // Attempt second fingerprint capture
}

// ===== CONVERT SECOND IMAGE TO TEMPLATE =====
p = finger.image2Tz(2);                   // Convert second image to template, store in slot 2
if (p != FINGERPRINT_OK) return p;        // If conversion failed, return error

// ===== CREATE FINGERPRINT MODEL FROM TWO TEMPLATES =====
// Combine the two template images to create final fingerprint model
p = finger.createModel();                 // Create model from both templates
if (p != FINGERPRINT_OK) return p;        // If model creation failed, return error

// ===== STORE MODEL IN SENSOR DATABASE =====
// Save the created model to sensor memory with unique ID
p = finger.storeModel(id);                // Save fingerprint model with assigned ID

return p;                                 // Return status code (FINGERPRINT_OK if success)
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: Fingerprint Enrollment System
  DESCRIPTION: Arduino-based system for batch enrollment of student fingerprints.
               Automatically assigns unique IDs to each enrolled student and
               stores fingerprints in sensor database for later attendance
               matching or identification.
  
  COMPONENTS USED:
  - Arduino UNO Microcontroller
  - Adafruit R307 Fingerprint Sensor Module (57600 baud)
  - USB Serial connection for prompts/feedback
  
  PIN CONFIGURATION:
  - Pin 2: Software Serial RX (fingerprint sensor data in)
  - Pin 3: Software Serial TX (fingerprint sensor data out)
  
  ENROLLMENT PROCESS:
  1. System prompts "Place finger to register"
  2. Student places finger on sensor
  3. System captures first fingerprint image
  4. System prompts "Remove finger"
  5. Student lifts finger for 2 seconds
  6. System prompts "Place same finger again"
  7. Student places same finger on sensor
  8. System captures second fingerprint image
  9. System creates fingerprint model from both scans
  10. System stores model with current Student ID
  11. System increments Student ID and repeats from step 1
  
  ENROLLMENT DURATION:
  - Per student: ~10-15 seconds
  - For 100 students: ~20-30 minutes
  - Sensor warm-up: 30-60 seconds initially
  
  SENSOR DATABASE CAPACITY:
  - Standard R307: 162 fingerprints (modules vary)
  - ID range: 1-162
  - After reaching capacity: Old fingerprints must be deleted
  
  AUTO-INCREMENT STUDENT ID:
  - Starts at ID 1
  - Increments by 1 after each successful enrollment
  - ID 1 = first enrolled student
  - ID 2 = second enrolled student
  - etc.
  
  DUAL-SCAN ENROLLMENT:
  Why two scans?
  - First scan: Captures fingerprint characteristics
  - Second scan: Verifies consistency and improves accuracy
  - Combined model: More reliable for matching later
  - Reduces false negatives: Student not recognized
  - Reduces false positives: Wrong student recognized
  
  ERROR HANDLING:
  - If first capture fails: Loop until successful
  - If finger not removed: System detects and asks again
  - If second capture fails: Return error to main loop
  - If model creation fails: Return error to main loop
  - If storage fails: Return error to main loop
  
  SERIAL OUTPUT EXAMPLES:
  "Ready to enroll students" = System initialized
  "Place finger to register" = Ready for enrollment
  "Remove finger" = Waiting for finger removal
  "Place same finger again" = Ready for second scan
  "Student saved with ID: 1" = ID 1 enrolled successfully
  "Failed to register" = Enrollment failed, try again
  
  TYPICAL ENROLLMENT SEQUENCE:
  ID 1: Student places finger twice → "Student saved with ID: 1"
  ID 2: Next student places finger twice → "Student saved with ID: 2"
  ID 3: Next student places finger twice → "Student saved with ID: 3"
  ... and so on
  
  FEATURES:
  - Automated ID assignment
  - Real-time enrollment feedback
  - Dual-scan for accuracy
  - Serial monitor tracking
  - Simple one-command enrollment
  - Error detection and reporting
  
  IMPROVEMENTS:
  - Add LCD display for visual feedback
  - Add buzzer for audio confirmation
  - Add temperature compensation
  - Store enrollment list on SD card
  - Add web interface for enrollment management
  - Implement batch delete function
  
  USAGE:
  1. Upload sketch to Arduino
  2. Open Serial Monitor at 9600 baud
  3. Wait for "Ready to enroll students" message
  4. Each student places finger when prompted
  5. System stores fingerprint with auto-assigned ID
  6. Repeat for all students
  7. Sensor stores all fingerprints for later attendance
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/