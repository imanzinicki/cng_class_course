/*================================================================================
  PIR MOTION SENSOR WITH BUZZER ALERT
  ESP8266 motion detection system using HC-SR501 PIR sensor
  Features: Real-time motion alerts, buzzer, serial monitoring
================================================================================*/

// ===== PIR SENSOR PIN DEFINITION =====
#define PIR_PIN D5                        // PIR sensor input on GPIO14 (D5) - detects motion

// ===== BUZZER PIN DEFINITION =====
#define BUZZER_PIN D6                     // Buzzer output on GPIO12 (D6) - sounds alarm on motion

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== PIR SENSOR PIN CONFIGURATION =====
  pinMode(PIR_PIN, INPUT);                // Set PIR sensor as INPUT to read motion detection
  
  // ===== BUZZER PIN CONFIGURATION =====
  pinMode(BUZZER_PIN, OUTPUT);            // Set buzzer as OUTPUT to control alarm
  
  // ===== SERIAL COMMUNICATION =====
  Serial.begin(115200);                   // Initialize serial at 115200 baud (ESP8266 standard)
  Serial.println("PIR Motion Sensor with Buzzer Initialized"); // Status message
}

// ===== MAIN LOOP =====
void loop() {
  // ===== READ PIR SENSOR STATE =====
  int motion = digitalRead(PIR_PIN);      // Read sensor: HIGH = motion detected, LOW = no motion

  // ===== CHECK FOR MOTION DETECTION =====
  if (motion == HIGH) {                   // If motion detected (sensor output HIGH)
    digitalWrite(BUZZER_PIN, HIGH);       // Turn buzzer ON
    Serial.println("Motion Detected! Buzzer ON"); // Print alert message
  } else {                                // If no motion (sensor output LOW)
    digitalWrite(BUZZER_PIN, LOW);        // Turn buzzer OFF
    Serial.println("No Motion. Buzzer OFF"); // Print normal status
  }
  
  delay(500);                             // Check every 500ms
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: PIR Motion Sensor with Buzzer Alert
  DESCRIPTION: Security system that detects motion using a passive infrared (PIR)
               sensor and sounds a buzzer alarm when motion is detected. Useful
               for intruder alerts, motion-activated automation, or occupancy
               detection.
  
  COMPONENTS USED:
  - ESP8266 WiFi Module (NodeMCU or similar)
  - HC-SR501 PIR Motion Sensor (passive infrared detection)
  - Active Buzzer (5V, produces sound alarm)
  
  PIN CONFIGURATION:
  - D5 (GPIO14): PIR sensor data input (HIGH = motion, LOW = no motion)
  - D6 (GPIO12): Buzzer control output (HIGH = buzzer on, LOW = buzzer off)
  
  SENSOR CHARACTERISTICS:
  - Detection range: 3-7 meters (configurable)
  - Response time: 1-2 seconds
  - Warm-up time: 30-60 seconds after power-on
  - Output: HIGH when motion detected, LOW otherwise
  - Adjustable sensitivity and delay via onboard potentiometers
  
  FEATURES:
  - Real-time motion detection and response
  - Immediate buzzer activation on motion
  - Serial monitor output for monitoring
  - Simple binary logic (motion/no motion)
  - Low power consumption
  
  USAGE:
  1. Upload sketch to ESP8266
  2. Allow 60 seconds warm-up time after power-on
  3. Open Serial Monitor at 115200 baud
  4. Move in front of sensor to trigger motion
  5. Buzzer sounds and message displays
  6. Standing still silences alarm after ~2 seconds
  
  CALIBRATION:
  Most HC-SR501 sensors have adjustable parameters:
  - Sensitivity potentiometer: Adjust detection range
  - Delay potentiometer: Adjust how long buzzer sounds
  - Time adjustment: Pin jumper settings (not implemented in this sketch)
  
  TYPICAL OPERATION:
  - No motion: Buzzer OFF, prints status every 500ms
  - Motion detected: Buzzer ON, prints alert message
  - Motion stopped: Buzzer OFF after ~2 seconds
  
  TROUBLESHOOTING:
  - No response to motion: Check sensor warm-up (60 sec), verify pin connections
  - Buzzer always on: Check pin logic, sensitivity may need adjustment
  - False triggers: Reduce sensitivity or avoid heat sources near sensor
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/