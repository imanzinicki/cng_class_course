/*================================================================================
  ADVANCED ULTRASONIC DISTANCE MEASUREMENT WITH ALERTS
  Arduino UNO with HC-SR04 sensor, RGB LED status, and buzzer alarm
  Features: Distance measurement, threat level detection, visual/audio alerts
================================================================================*/

// ===== ULTRASONIC SENSOR PIN DEFINITIONS =====
int trig = 9;                             // Trigger pin for HC-SR04 on pin 9
int echo = 10;                            // Echo pin for HC-SR04 on pin 10

// ===== ALARM/INDICATOR PIN DEFINITIONS =====
int buzzer = 5;                           // Buzzer alarm on pin 5 (PWM capable)
int red=2;                                // Red LED on pin 2 (danger indicator)
int green=6;                              // Green LED on pin 6 (safe indicator)

// ===== GLOBAL VARIABLES FOR DISTANCE CALCULATION =====
long duration;                            // Stores echo pulse duration in microseconds
int distance;                             // Stores calculated distance in centimeters

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== LED PIN MODES =====
  pinMode(red, OUTPUT);                   // Set red LED as OUTPUT
  pinMode(green, OUTPUT);                 // Set green LED as OUTPUT
  
  // ===== ULTRASONIC SENSOR PIN MODES =====
  pinMode(trig, OUTPUT);                  // Set trigger pin as OUTPUT
  pinMode(echo, INPUT);                   // Set echo pin as INPUT
  
  // ===== BUZZER PIN MODE =====
  pinMode(buzzer, OUTPUT);                // Set buzzer as OUTPUT
  
  // ===== SERIAL COMMUNICATION =====
  Serial.begin(9600);                     // Initialize serial at 9600 baud
}

// ===== MAIN LOOP =====
void loop() {
  // ===== SEND TRIGGER PULSE =====
  digitalWrite(trig, LOW);                // Set trigger LOW
  delayMicroseconds(2);                   // Wait 2 microseconds

  digitalWrite(trig, HIGH);               // Set trigger HIGH
  delayMicroseconds(10);                  // Keep HIGH for 10 microseconds
  
  digitalWrite(trig, LOW);                // Set trigger LOW

  // ===== MEASURE ECHO PULSE =====
  duration = pulseIn(echo, HIGH);         // Measure echo pulse duration
  
  // ===== CALCULATE DISTANCE =====
  distance = duration * 0.034 / 2;        // Convert to centimeters

  // ===== DISPLAY DISTANCE =====
  Serial.println(distance);               // Print distance value

  // ===== THREAT LEVEL DETECTION AND RESPONSE =====
  
  // WARNING ZONE (30-60cm) - Yellow alert
  if(distance>30 & distance <60){         // If distance between 30-60cm
    digitalWrite(buzzer, HIGH);           // Activate buzzer
    digitalWrite(red, HIGH);              // Turn ON red LED (warning)
    digitalWrite(green,LOW);              // Turn OFF green LED
  }
  // DANGER ZONE (<30cm) - Red alert with pulsing buzzer
  else if(distance<30){                   // If distance less than 30cm (critical)
    digitalWrite(buzzer,HIGH);            // Activate buzzer
    delay(1000);                          // Buzzer ON for 1 second
    digitalWrite(red, HIGH);              // Turn ON red LED (danger)
    digitalWrite(green,LOW);              // Turn OFF green LED
  }
  // SAFE ZONE (>60cm) - All clear
  else{                                   // If distance greater than 60cm
    digitalWrite(red, HIGH);              // Keep red LED on (standby)
    digitalWrite(green,LOW);              // Turn OFF green LED
    digitalWrite(buzzer, LOW);            // Turn OFF buzzer (no threat)
  }

  delay(500);                             // Wait 500ms before next measurement
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: Advanced Ultrasonic Distance Measurement with Alerts
  DESCRIPTION: Proximity detection system that measures distance and provides
               visual (RGB LEDs) and audio (buzzer) alerts based on threat
               levels. Useful for collision avoidance or intrusion detection.
  
  COMPONENTS USED:
  - Arduino UNO Microcontroller
  - HC-SR04 Ultrasonic Distance Sensor (range: 2cm - 400cm)
  - Red LED (danger/warning indicator)
  - Green LED (safe indicator)
  - Active Buzzer (audio alert)
  
  PIN CONFIGURATION:
  - Pin 2 (D2): Red LED output (danger zone)
  - Pin 5 (D5): Buzzer output (PWM capable, alarm sound)
  - Pin 6 (D6): Green LED output (safe zone)
  - Pin 9 (D9): Ultrasonic trigger output
  - Pin 10 (D10): Ultrasonic echo input
  
  THREAT LEVELS:
  1. SAFE ZONE (>60cm): Green OK
     - No buzzer
     - Both LEDs off (red stays on for visibility)
     - Safe to proceed
  
  2. WARNING ZONE (30-60cm): Yellow/Red caution
     - Continuous buzzer
     - Red LED ON, Green LED OFF
     - Object approaching, reduce speed
  
  3. DANGER ZONE (<30cm): Red Critical
     - Pulsing buzzer (1 sec on/off)
     - Red LED ON, Green LED OFF
     - Immediate action required
  
  DISTANCE REFERENCE:
  - 10cm: Very close (arm's length)
  - 30cm: Close warning zone
  - 60cm: Safe distance
  - 100cm: Far away (1 meter)
  - 200cm: Very far (2 meters)
  - 400cm: Maximum range
  
  FEATURES:
  - Real-time distance measurement
  - Three-level threat detection
  - Visual indicators (red/green LED)
  - Audio alarm (buzzer)
  - Serial output for monitoring
  - 500ms update interval
  
  APPLICATIONS:
  - Collision avoidance for robots
  - Intruder detection system
  - Parking sensor backup alarm
  - Proximity-based automation
  - Motion/approach monitoring
  
  USAGE:
  1. Mount HC-SR04 sensor pointing forward
  2. Upload sketch to Arduino
  3. Open Serial Monitor at 9600 baud
  4. Move object in front of sensor
  5. Observe LED changes and buzzer response
  6. Distance printed to serial every 500ms
  
  BUZZER BEHAVIOR:
  - Continuous buzzer: 30-60cm range (warning)
  - Pulsed buzzer: <30cm range (1 sec on/off - danger)
  - Off: >60cm range (safe)
  
  IMPROVEMENTS:
  - Implement moving average for smoother readings
  - Add adjustable threshold values
  - Implement data logging
  - Add EEPROM storage for calibration
  - Implement multi-sensor array
  
  TROUBLESHOOTING:
  - Incorrect distances: Calibrate ultrasonic sensor
  - No buzzer sound: Check pin 5 connections
  - LED doesn't respond: Check pin modes
  - Serial shows 0: Check echo pin timeout
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/