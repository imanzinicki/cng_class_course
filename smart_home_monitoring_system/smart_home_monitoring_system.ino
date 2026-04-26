#define BLYNK_TEMPLATE_ID "TMPL2Kurxuc6o"
#define BLYNK_TEMPLATE_NAME "smart home monitoring system"
#define BLYNK_AUTH_TOKEN "Xq1FtDJTeF_n6kFS0ISOqSldKbPjTAbM"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>


#define IR_PIN D5       
#define LED_PIN D1      


#define RED_PIN D6
#define GREEN_PIN D7
#define BLUE_PIN D8

#define SERVO_PIN D4    

char ssid[] = "1CNG THECADEMIE LTD";
char pass[] = "cng12345678";

Servo myServo;

bool doorLightState = false;


BLYNK_WRITE(V2) {       
  doorLightState = param.asInt();
  updateDoorAndLED();
}


BLYNK_WRITE(V6) {
  int r = param[0].asInt();  
  int g = param[1].asInt();
  int b = param[2].asInt();


  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);


}


void updateDoorAndLED() {
  if (doorLightState) {
    digitalWrite(LED_PIN, HIGH);
    myServo.write(90);   
  } else {
    digitalWrite(LED_PIN, LOW);
    myServo.write(0);    
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(IR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);


  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);


  analogWriteRange(255);

  myServo.attach(SERVO_PIN);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  updateDoorAndLED();
}

void loop() {
  Blynk.run();


  int irState = digitalRead(IR_PIN);
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