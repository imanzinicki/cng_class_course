/*================================================================================
  POTENTIOMETER TO LED BRIGHTNESS CONTROL
  Arduino PWM LED brightness controlled by variable resistor (potentiometer)
  Features: Analog input reading, PWM output, real-time brightness adjustment
================================================================================*/

// ===== LED PIN DEFINITION =====
int ledPin = 9;                           // LED on pin 9 (PWM capable: 3,5,6,9,10,11)
                  
// ===== LED BRIGHTNESS VARIABLE =====
int ledBrightness;                        // Stores LED brightness value (0-255)

// ===== POTENTIOMETER PIN DEFINITION =====
int potpin=8;                             // Potentiometer analog input on pin 8 (A0-A5)

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== LED PIN MODE =====
  pinMode(ledPin, OUTPUT);                // Set LED pin as OUTPUT for PWM control
  
  // Note: Potentiometer pin does not need pinMode setup (analog pins default to INPUT)
}

// ===== MAIN LOOP =====
void loop() {
  // ===== READ POTENTIOMETER VALUE =====
  int potValue = analogRead(potpin);      // Read analog value (0-1023, 10-bit ADC)
         
  // ===== MAP POTENTIOMETER RANGE TO LED BRIGHTNESS =====
  // Convert 0-1023 range to 0-255 range for PWM
  ledBrightness = map(potValue, 0, 1023, 0, 255); // Map analog reading to PWM output range
  
  // ===== SET LED BRIGHTNESS =====
  analogWrite(ledPin, ledBrightness);     // Write PWM value to LED (0=off, 255=full brightness)
          
  delay(10);                              // Wait 10ms for debouncing and smooth response
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: Potentiometer to LED Brightness Control
  DESCRIPTION: Simple project that demonstrates analog input reading and PWM
               output control. Rotating potentiometer changes LED brightness
               in real-time. Great for learning analog I/O and PWM concepts.
  
  COMPONENTS USED:
  - Arduino UNO (or compatible board)
  - 10kΩ Potentiometer (variable resistor)
  - LED (any color)
  - 220Ω Resistor (current limiting for LED)
  - Breadboard and jumper wires
  
  PIN CONFIGURATION:
  - Pin 8 (Analog A0): Potentiometer input
  - Pin 9 (PWM): LED output (must support PWM: 3,5,6,9,10,11)
  
  POTENTIOMETER WIRING:
  - One end: 5V power
  - Middle (wiper): Analog pin 8 (A0)
  - Other end: GND (ground)
  
  LED WIRING:
  - Long leg (anode): Pin 9 via 220Ω resistor
  - Short leg (cathode): GND (ground)
  
  COMPONENT SPECIFICATIONS:
  
  Potentiometer:
  - Type: 10kΩ variable resistor (rotary potentiometer)
  - Resistance range: 0Ω - 10kΩ
  - Used as voltage divider
  - Output voltage: 0V (fully counterclockwise) to 5V (fully clockwise)
  
  ADC (Analog to Digital Converter):
  - Resolution: 10-bit (0-1023)
  - Arduino reads voltage 0-5V as values 0-1023
  - Conversion: 0-1023 = 0V-5V
  
  PWM (Pulse Width Modulation):
  - Resolution: 8-bit (0-255)
  - Duty cycle: 0% (off) to 100% (full brightness)
  - Frequency: ~490Hz (standard Arduino)
  
  OPERATION:
  1. Potentiometer is turned, changing voltage divider output
  2. Arduino reads analog voltage as 0-1023 value
  3. Value mapped to 0-255 for LED brightness
  4. PWM signal sent to LED pin
  5. LED brightness follows potentiometer position
  
  BRIGHTNESS MAPPING:
  - Potentiometer 0° (fully CCW): analogRead = 0 → brightness 0 (OFF)
  - Potentiometer 90°: analogRead = 512 → brightness 128 (half brightness)
  - Potentiometer 180° (fully CW): analogRead = 1023 → brightness 255 (FULL)
  
  FEATURES:
  - Real-time brightness control
  - Full range 0-100% brightness
  - Smooth response (10ms update)
  - Simple circuit
  - Perfect for learning
  
  ARDUINO PWM PINS:
  - UNO: 3, 5, 6, 9, 10, 11
  - MEGA: 2-13, 44-46
  - Leonardo: 3, 5, 6, 9, 10, 11, 13
  
  IMPROVEMENTS:
  - Add multiple LEDs (RGB LED control)
  - Add button to switch between modes
  - Store brightness preference in EEPROM
  - Add fade-in/fade-out animation
  - Implement timer-based on/off
  
  TROUBLESHOOTING:
  - LED doesn't turn on: Check wiring and 220Ω resistor
  - LED always full brightness: Check potentiometer wiring
  - Erratic brightness: Check potentiometer potcore or loose connection
  - No response to turning potentiometer: Verify pin configuration
  
  TYPICAL BRIGHTNESS LEVELS:
  - 0-50: Very dim (night light)
  - 50-100: Dim (reading light)
  - 100-200: Medium (desk work)
  - 200-255: Bright (full illumination)
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/