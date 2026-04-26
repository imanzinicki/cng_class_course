/*================================================================================
  SMART HOME MONITORING SYSTEM
  ESP8266 with Blynk IoT Cloud Control
  Features: Door control, LED status, RGB LED control, IR sensor monitoring
  Blynk Virtual Pins: V0(IR), V2(Door), V6(RGB)
================================================================================*/

// ===== BLYNK AUTHENTICATION CREDENTIALS =====
// These tokens connect to Blynk IoT cloud for remote control
#define BLYNK_TEMPLATE_ID "TMPL2Kurxuc6o"          // Blynk template unique identifier
#define BLYNK_TEMPLATE_NAME "smart home monitoring system" // Project name in Blynk
#define BLYNK_AUTH_TOKEN "Xq1FtDJTeF_n6kFS0ISOqSldKbPjTAbM" // Authorization token for Blynk

// ===== BLYNK DEBUG OUTPUT =====
#define BLYNK_PRINT Serial                // Enable serial debug output for Blynk

// ===== LIBRARY INCLUDES =====
#include <ESP8266WiFi.h>                  // WiFi library for ESP8266
#include <BlynkSimpleEsp8266.h>           // Blynk library for IoT cloud integration
#include <Servo.h>                        // Servo motor control library

// ===== PIN DEFINITIONS FOR SENSORS AND ACTUATORS =====
#define IR_PIN D5                         // Infrared sensor on GPIO14 (D5) for motion detection
#define LED_PIN D1                        // LED status indicator on GPIO5 (D1)

// ===== RGB LED PIN DEFINITIONS =====
#define RED_PIN D6                        // Red LED component on GPIO12 (D6)
#define GREEN_PIN D7                      // Green LED component on GPIO13 (D7)
#define BLUE_PIN D8                       // Blue LED component on GPIO15 (D8)

// ===== SERVO MOTOR PIN =====
#define SERVO_PIN D4                      // Servo motor control on GPIO2 (D4) for door lock

// ===== WiFi CREDENTIALS =====
char ssid[] = "1CNG THECADEMIE LTD";       // WiFi network name (SSID)
char pass[] = "cng12345678";               // WiFi network password

// ===== SERVO OBJECT CREATION =====
Servo myServo;                            // Servo object to control door lock mechanism

// ===== GLOBAL STATE VARIABLE =====
bool doorLightState = false;              // Stores door lock state (false = closed, true = open)

// ===== BLYNK VIRTUAL PIN V2 HANDLER (Door Control) =====
// This function is triggered when Blynk app button on V2 is pressed
BLYNK_WRITE(V2) {                         // V2 = Virtual pin for door control button
  doorLightState = param.asInt();         // Read button state from Blynk (0 or 1)
  updateDoorAndLED();                     // Update physical door and LED based on state
}

// ===== BLYNK VIRTUAL PIN V6 HANDLER (RGB LED Control) =====
// This function handles RGB color slider input from Blynk app
BLYNK_WRITE(V6) {                         // V6 = Virtual pin for RGB color slider
  int r = param[0].asInt();               // Extract RED value (0-255) from Blynk slider
  int g = param[1].asInt();               // Extract GREEN value (0-255) from Blynk slider
  int b = param[2].asInt();               // Extract BLUE value (0-255) from Blynk slider

  // Apply PWM (Pulse Width Modulation) to RGB pins for color mixing
  analogWrite(RED_PIN, r);                // Set red LED brightness using PWM
  analogWrite(GREEN_PIN, g);              // Set green LED brightness using PWM
  analogWrite(BLUE_PIN, b);               // Set blue LED brightness using PWM
}

// ===== FUNCTION: UPDATE DOOR AND LED STATUS =====
// Controls servo position and LED based on door state
void updateDoorAndLED() {
  if (doorLightState) {                   // If door should be OPEN
    digitalWrite(LED_PIN, HIGH);          // Turn ON LED indicator
    myServo.write(90);                    // Set servo to 90 degrees (open position)
  } else {                                // If door should be CLOSED
    digitalWrite(LED_PIN, LOW);           // Turn OFF LED indicator
    myServo.write(0);                     // Set servo to 0 degrees (closed position)
  }
}

// ===== SETUP FUNCTION (Initialization) =====
// Runs once when Arduino starts up
void setup() {
  // ===== SERIAL COMMUNICATION =====
  Serial.begin(9600);                     // Initialize serial for debugging at 9600 baud rate

  // ===== SENSOR PIN INITIALIZATION =====
  pinMode(IR_PIN, INPUT);                 // Set IR sensor as INPUT to read motion data
  pinMode(LED_PIN, OUTPUT);               // Set LED as OUTPUT to control brightness

  // ===== RGB LED PIN INITIALIZATION =====
  pinMode(RED_PIN, OUTPUT);               // Set red LED pin as OUTPUT for PWM control
  pinMode(GREEN_PIN, OUTPUT);             // Set green LED pin as OUTPUT for PWM control
  pinMode(BLUE_PIN, OUTPUT);              // Set blue LED pin as OUTPUT for PWM control

  // ===== PWM CONFIGURATION =====
  analogWriteRange(255);                  // Set PWM range to 0-255 (standard 8-bit)

  // ===== SERVO INITIALIZATION =====
  myServo.attach(SERVO_PIN);              // Attach servo object to GPIO pin for control

  // ===== BLYNK CONNECTION =====
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Connect to Blynk cloud with credentials

  // ===== SET INITIAL STATE =====
  updateDoorAndLED();                     // Apply initial door/LED state on startup
}

// ===== MAIN LOOP =====
// Runs repeatedly after setup()
void loop() {
  // ===== BLYNK CONNECTION MAINTENANCE =====
  Blynk.run();                            // Maintain Blynk connection and handle requests

  // ===== READ IR SENSOR =====
  int irState = digitalRead(IR_PIN);      // Read IR sensor value (HIGH = motion, LOW = no motion)
  
  // ===== SEND DATA TO BLYNK =====
  Blynk.virtualWrite(V0, irState);        // Send IR sensor state to Blynk virtual pin V0
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: Smart Home Monitoring System
  DESCRIPTION: ESP8266-based smart home system with Blynk IoT cloud integration.
               Allows remote control of door locks and RGB lighting via smartphone
               app. Monitors motion detection via IR sensor.
  
  COMPONENTS USED:
  - ESP8266 WiFi Module (NodeMCU or similar)
  - HC-SR501 IR Motion Sensor
  - SG90 Servo Motor (door lock mechanism)
  - RGB LED (common cathode)
  - Blynk IoT Cloud Platform
  
  PIN CONFIGURATION:
  - D1 (GPIO5): LED indicator output
  - D4 (GPIO2): Servo motor PWM control
  - D5 (GPIO14): IR motion sensor input
  - D6 (GPIO12): Red LED component output
  - D7 (GPIO13): Green LED component output
  - D8 (GPIO15): Blue LED component output
  
  BLYNK VIRTUAL PINS:
  - V0: IR sensor status display
  - V2: Door lock toggle button
  - V6: RGB color picker slider
  
  FEATURES:
  - Remote door lock/unlock via Blynk app
  - RGB lighting control with color picker
  - Real-time motion detection monitoring
  - LED status indicator
  - WiFi auto-reconnect
  
  USAGE:
  1. Configure WiFi SSID and password
  2. Add Blynk virtual pins in app (V0, V2, V6)
  3. Upload sketch to ESP8266
  4. Control via Blynk smartphone application
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/
}
}