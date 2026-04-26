#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(10, 11); 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  while (!Serial);  
  delay(100);

  Serial.println("Fingerprint sensor test");

  finger.begin(57600); 
  if (finger.verifyPassword()) {
    Serial.println("Sensor detected!");
  } else {
    Serial.println("Sensor not found :(");
    while (1) { delay(1); }
  }

  Serial.println("Ready to clear all fingerprints...");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'C') { 
      clearAllFingerprints();
    }
  }
}

void clearAllFingerprints() {
  Serial.println("Clearing all fingerprints...");
  int result = finger.emptyDatabase();
  if (result == FINGERPRINT_OK) {
    Serial.println("All fingerprints cleared successfully!");
  } else {
    Serial.print("Failed to clear database. Error code: ");
    Serial.println(result);
  }
}