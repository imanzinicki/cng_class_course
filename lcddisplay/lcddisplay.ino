/*================================================================================
  16x2 I2C LCD DISPLAY TEST
  Basic test of I2C LCD display with initialization and text display
  Features: Backlight control, cursor positioning, multi-line text
================================================================================*/

// ===== LIBRARY INCLUDES =====
#include <Wire.h>                         // I2C communication library
#include <LiquidCrystal_I2C.h>           // I2C LCD display library

// ===== LCD INITIALIZATION =====
// Address 0x27 is standard for most 16x2 I2C LCD modules
// 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);      // Create LCD object with address, width, height

// ===== SETUP FUNCTION (Initialization) =====
void setup() {
  // ===== LCD INITIALIZATION =====
  lcd.init();                             // Initialize LCD hardware and I2C communication
          
  // ===== ENABLE BACKLIGHT =====
  lcd.backlight();                        // Turn on LCD backlight (brightness control)

  // ===== DISPLAY LINE 1 =====
  lcd.setCursor(0, 0);                    // Position cursor: column 0, row 0 (top-left)
  lcd.print("Hello World");               // Print first line of text (max 16 chars)

  // ===== DISPLAY LINE 2 =====
  lcd.setCursor(0, 1);                    // Position cursor: column 0, row 1 (bottom-left)
  lcd.print("Minaj Tech");                // Print second line of text (max 16 chars)
  
  // ===== CLEAR DISPLAY =====
  lcd.clear();                            // Clear all text from LCD display
}

// ===== MAIN LOOP =====
void loop() {
  // Empty - Display is initialized in setup and remains on
}

/*================================================================================
  FILE DOCUMENTATION
  
  PROJECT: 16x2 I2C LCD Display Test
  DESCRIPTION: Simple test sketch for 16x2 character LCD display with I2C
               interface. Demonstrates initialization, text display,
               cursor positioning, and backlight control.
  
  COMPONENTS USED:
  - Arduino UNO or compatible microcontroller
  - 16x2 I2C LCD Display Module (address 0x27 typical)
  - I2C backpack (usually pre-soldered on LCD module)
  
  PIN CONFIGURATION:
  - SDA (A4): I2C data line to LCD backpack
  - SCL (A5): I2C clock line to LCD backpack
  - VCC: 5V power
  - GND: Ground
  
  TYPICAL I2C ADDRESSES:
  - Most common: 0x27 (used in this sketch)
  - Alternative: 0x3F (less common)
  - To find your LCD address: Run I2C scanner sketch
  
  DISPLAY SPECIFICATIONS:
  - Resolution: 16 characters x 2 rows
  - Character type: 5x8 pixel matrix
  - Display type: LCD (Liquid Crystal Display)
  - Interface: I2C (reduces wiring to 2 lines)
  - Backlight: Usually blue or green LED
  
  LIBRARY FUNCTIONS USED:
  - lcd.init(): Initialize LCD hardware
  - lcd.backlight(): Turn on backlight (call noBacklight() to turn off)
  - lcd.setCursor(col, row): Position cursor for next text
  - lcd.print("text"): Display text at cursor position
  - lcd.clear(): Erase all text and reset cursor
  
  CURSOR POSITIONS:
  Row 0 (top):    columns 0-15
  Row 1 (bottom): columns 0-15
  Example: lcd.setCursor(5, 0) = row 0, column 5
  
  FEATURES:
  - 16x2 character display capacity
  - I2C interface (4-wire: SDA, SCL, VCC, GND)
  - Adjustable contrast potentiometer
  - Backlight control
  - Custom character support (up to 8 custom chars)
  
  COMMON FUNCTIONS:
  - lcd.write(byte): Display custom character by code
  - lcd.blink(): Enable cursor blinking
  - lcd.noBlink(): Disable cursor
  - lcd.cursor(): Show cursor
  - lcd.noCursor(): Hide cursor
  - lcd.display(): Turn display ON
  - lcd.noDisplay(): Turn display OFF (backlight stays on)
  - lcd.scrollDisplayLeft(): Scroll text left
  - lcd.scrollDisplayRight(): Scroll text right
  
  TROUBLESHOOTING:
  - No display: Check I2C address (0x27 vs 0x3F)
  - No backlight: Check backlight potentiometer on I2C module
  - Garbled text: Check I2C connection quality
  - Blank display: Adjust contrast potentiometer on backpack
  
  USAGE:
  1. Connect LCD I2C to Arduino (SDA=A4, SCL=A5)
  2. Upload sketch
  3. LCD displays "Hello World" and "Minaj Tech" briefly
  4. Display clears after initialization
  5. Modify setup() to create desired display behavior
  
  AUTHOR: IMANZIYURUKUNDO NICK
  PHONE: 0793540248
  GITHUB: https://github.com/imanzinicki/cng_class_course
  DATE: April 2026
  VERSION: 1.0
================================================================================*/
