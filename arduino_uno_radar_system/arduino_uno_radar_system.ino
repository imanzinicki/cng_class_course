/*================================================================================
  ARDUINO UNO RADAR SYSTEM WITH ULTRASONIC SENSOR
  Features: 180° servo sweep, HC-SR04 distance detection, RGB status, buzzer alerts
  Range: 2cm to 400cm, automatic threat level detection
================================================================================*/

// ===== LIBRARY INCLUDES =====
#include <Servo.h>                        // Servo motor control library

// ===== ULTRASONIC SENSOR PIN DEFINITIONS =====
#define TRIG 9                            // Trigger pin for HC-SR04 ultrasonic sensor
#define ECHO 10                           // Echo pin for HC-SR04 ultrasonic sensor

// ===== RGB LED PIN DEFINITIONS =====
#define RED 3                             // Red LED on pin 3 (PWM capable)
#define GREEN 4                           // Green LED on pin 4 (PWM capable)
#define BLUE 5                            // Blue LED on pin 5 (PWM capable)

// ===== BUZZER PIN =====
#define BUZZER 7                          // Buzzer alarm on pin 7

// ===== SERVO OBJECT =====
Servo myServo;                            // Servo object for 180° rotation sweep

// ===== GLOBAL VARIABLES FOR DISTANCE MEASUREMENT =====
long duration;                            // Stores echo pulse duration in microseconds
int distance;                             // Stores calculated distance in centimeters

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== ULTRASONIC SENSOR PIN MODES =====
  pinMode(TRIG, OUTPUT);                  // Set trigger pin as OUTPUT (sends 10µs pulse)
  pinMode(ECHO, INPUT);                   // Set echo pin as INPUT (receives return pulse)

  // ===== RGB LED PIN MODES =====
  pinMode(RED, OUTPUT);                   // Set red LED as OUTPUT
  pinMode(GREEN, OUTPUT);                 // Set green LED as OUTPUT
  pinMode(BLUE, OUTPUT);                  // Set blue LED as OUTPUT

  // ===== BUZZER PIN MODE =====
  pinMode(BUZZER, OUTPUT);                // Set buzzer as OUTPUT for alarm control

  // ===== SERVO INITIALIZATION =====
  myServo.attach(6);                     // Attach servo to pin 6 (PWM required)

  // ===== SERIAL COMMUNICATION =====
  Serial.begin(9600);                     // Initialize serial at 9600 baud for data output
}

// ===== FUNCTION: GET DISTANCE FROM ULTRASONIC SENSOR =====
// Sends trigger pulse and measures echo time to calculate distance
int getDistance() {
  // ===== SEND TRIGGER PULSE =====
  digitalWrite(TRIG, LOW);                // Set trigger LOW before sending pulse
  delayMicroseconds(2);                   // Wait 2 microseconds
  
  digitalWrite(TRIG, HIGH);               // Set trigger HIGH to start 10µs pulse
  delayMicroseconds(10);                  // Keep HIGH for 10 microseconds (sensor requirement)
  
  digitalWrite(TRIG, LOW);                // Set trigger LOW to end pulse

  // ===== MEASURE ECHO PULSE =====
  duration = pulseIn(ECHO, HIGH, 30000); // Measure echo pulse duration in microseconds (max 30ms timeout)
  
  // ===== CALCULATE DISTANCE =====
  // Formula: distance = (duration in µs × speed of sound) / 2
  // Speed of sound = 343 m/s = 0.0343 cm/µs
  distance = duration * 0.034 / 2;        // Convert time to distance in centimeters

  return distance;                        // Return calculated distance value
}

// ===== FUNCTION: HANDLE THREAT LEVEL ALERTS =====
// Controls RGB color and buzzer based on object distance
void handleAlert(int d) {
  // ===== SAFE DISTANCE (>50cm) - GREEN =====
  if (d > 50) {
    digitalWrite(RED, LOW);               // Turn OFF red LED
    digitalWrite(GREEN, HIGH);            // Turn ON green LED (safe)
    digitalWrite(BLUE, LOW);              // Turn OFF blue LED
    digitalWrite(BUZZER, LOW);            // Turn OFF buzzer (no threat)
  } 
  // ===== WARNING DISTANCE (20-50cm) - BLUE + INTERMITTENT BUZZER =====
  else if (d > 20) {
    digitalWrite(RED, LOW);               // Turn OFF red LED
    digitalWrite(GREEN, LOW);             // Turn OFF green LED
    digitalWrite(BLUE, HIGH);             // Turn ON blue LED (warning)
    // Buzzer pulses at 5Hz (300ms cycle, 150ms on, 150ms off)
    digitalWrite(BUZZER, millis() % 300 < 150); // Modulate buzzer with millis() timing
  } 
  // ===== DANGER DISTANCE (<20cm) - RED + FAST BUZZER =====
  else {
    digitalWrite(RED, HIGH);              // Turn ON red LED (danger)
    digitalWrite(GREEN, LOW);             // Turn OFF green LED
    digitalWrite(BLUE, LOW);              // Turn OFF blue LED
    // Buzzer pulses at 10Hz (150ms cycle, 75ms on, 75ms off)
    digitalWrite(BUZZER, millis() % 150 < 75); // Faster buzzer pulse pattern
  }
}

// ===== MAIN LOOP =====
void loop() {
  // ===== FORWARD SWEEP (0° to 180°) =====
  // Sweeps servo from left to right in 8-degree increments
  for (int angle = 0; angle <= 180; angle += 8) {
    myServo.write(angle);                 // Set servo to current angle

    int d = getDistance();                // Get distance reading from ultrasonic sensor

    // ===== SERIAL OUTPUT FOR VISUALIZATION =====
    Serial.print(angle);                  // Print servo angle
    Serial.print(",");                    // Print comma separator
    Serial.println(d);                    // Print distance value

    handleAlert(d);                       // Update RGB and buzzer based on distance

    delay(100);                           // Wait 100ms before next measurement
  }

  // ===== REVERSE SWEEP (180° to 0°) =====
  // Sweeps servo from right to left in 8-degree increments
  for (int angle = 180; angle >= 0; angle -= 8) {
    myServo.write(angle);                 // Set servo to current angle

    int d = getDistance();                // Get distance reading from ultrasonic sensor

    // ===== SERIAL OUTPUT FOR VISUALIZATION =====
    Serial.print(angle);                  // Print servo angle
    Serial.print(",");                    // Print comma separator
    Serial.println(d);                    // Print distance value

    handleAlert(d);                       // Update RGB and buzzer based on distance

    delay(100);                           // Wait 100ms before next measurement
  }
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: Arduino UNO Radar System
  DESCRIPTION: A 180-degree rotating radar system that detects objects using
               an HC-SR04 ultrasonic sensor and provides visual (RGB LED) and
               audio (buzzer) alerts based on distance threats.
  
  COMPONENTS USED:
  - Arduino UNO Microcontroller
  - HC-SR04 Ultrasonic Distance Sensor (range: 2cm - 400cm)
  - SG90 Servo Motor (180° rotation)
  - RGB LED (common cathode or anode)
  - Active Buzzer (5V)
  
  PIN CONFIGURATION:
  - Pin 3 (D3): Red LED output
  - Pin 4 (D4): Green LED output
  - Pin 5 (D5): Blue LED output
  - Pin 6 (D6): Servo motor PWM control
  - Pin 7 (D7): Buzzer output
  - Pin 9 (D9): Ultrasonic trigger output
  - Pin 10 (D10): Ultrasonic echo input
  
  THREAT LEVELS:
  1. SAFE (>50cm): Green LED, no buzzer
  2. WARNING (20-50cm): Blue LED, buzzer at 5Hz
  3. DANGER (<20cm): Red LED, buzzer at 10Hz
  
  FEATURES:
  - Continuous 180° sweep pattern (forward and reverse)
  - Real-time distance measurement via serial output
  - Color-coded threat visualization
  - Progressive audio alerts
  
  USAGE:
  - Power: 5V from Arduino
  - Baud Rate: 9600 (Serial Monitor)
  - Upload: Standard Arduino IDE
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/
}