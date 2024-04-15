
// You'll need to know the Product ID for your keypad.
// Here's a summary:
//   * PID3844 4x4 Matrix Keypad
//   * PID3845 3x4 Matrix Keypad
//   * PID1824 3x4 Phone-style Matrix Keypad
//   * PID1332 Membrane 1x4 Keypad
//   * PID419  Membrane 3x4 Matrix Keypad

#include "Adafruit_Keypad.h"
//On Arduino nano SDA pin is on A4 and SCL pin is A5
#include <Wire.h>
#include <hd44780.h> // include hd44780 library header file
#include <hd44780ioClass/hd44780_I2Cexp.h> // i/o expander/backpack class
hd44780_I2Cexp lcd; // auto detect backpack and pin mappings

// Defining custom characters HEX values
uint8_t heart[8] = {0x00,0x0a,0x1f,0x1f,0x0e,0x04,0x00,0x00};
uint8_t smile[8] = {0x00,0x0a,0x0a,0x00,0x00,0x11,0x0e,0x00};
uint8_t frown[8] = {0x00,0x00,0x0a,0x00,0x00,0x00,0x0e,0x11};

// define your specific keypad here via PID
#define KEYPAD_PID1824
// define your pins here

#define C1    2
#define C2    3
#define C3    4
#define R1    5
#define R2    6
#define R3    7
#define R4    8

// leave this import after the above configuration
#include "keypad_config.h"

// define password size
#define MAX_PASSWORD_LENGTH 13
#define RESET_CODE_LENGTH 10
// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

const char *master_password = "1359640325760";
const char *reset_code = "***6969***";
const char *empty_buffer = "---";
char *buffer = (char *) malloc(sizeof(char) * MAX_PASSWORD_LENGTH);
char *password = (char *) malloc(sizeof(char) * MAX_PASSWORD_LENGTH);
int cursor_x = 0;
bool is_valid_passcode = false;
bool reset_code_entered = false;

// Define methods that will be used
void get_password();
bool validate_password();
void success_message();
void set_password();
void reset_lcd();
void initial_message();
void bad_passcode_msg();
void set_new_password();

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// initialize the library with the numbers of the interface pins


void setup() {
  int status;

	// initialize LCD with number of columns and rows: 
	// hd44780 returns a status from begin() that can be used
	// to determine if initalization failed.
	// the actual status codes are defined in <hd44780.h>
	// See the values RV_XXXX

	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		// begin() failed so blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

  Serial.begin(9600);
  customKeypad.begin();
  // set up the LCD's number of columns and rows:

  // create a new character
  lcd.createChar(0, heart);
  lcd.createChar(1, smile);
  lcd.createChar(2, frown);

  // Initialize buffer that will be saved to memory
  strcpy(password, empty_buffer);
  strcpy(buffer, empty_buffer);
  initial_message();
}

void loop() {
  get_password();
  while(is_valid_passcode){
    success_message();
    delay(10000); //10 second delay
  }
  delay(10);
}

// Ask for password input
void get_password()
{
  lcd.setCursor(cursor_x, 1);
  customKeypad.tick();
  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    char key = (char)e.bit.KEY;
    if(e.bit.EVENT == KEY_JUST_PRESSED && key != '#'){
      buffer[cursor_x] = key;
      cursor_x++;
      Serial.print("curr_pos: ");
      Serial.println(cursor_x);
      lcd.write(key);
    } 
    if(cursor_x == MAX_PASSWORD_LENGTH || key == '#')
    {
      Serial.println(buffer);
      is_valid_passcode = validate_password();

      if(reset_code_entered){
        set_new_password();
      } 
      else if(is_valid_passcode){
        // lcd.clear();
        // lcd.setCursor(0,0);
        // lcd.print("Success! ");
        // lcd.write(1);
        // Serial.println("Success!");
        cursor_x = 0;
        break;
      } 
      else{
        bad_passcode_msg();
        reset_lcd();
        cursor_x = 0;
      }
    }
  }
}

// Check if the password is correct
bool validate_password()
{
  // Does validation for the reset code. Will not work for this version
  // for(int i = 0; i < RESET_CODE_LENGTH; i++){
  //   if(buffer[i] != reset_code[i]){
  //     break;
  //   }
  //   if(i == 9){
  //     reset_code_entered = true;
  //     return false;
  //   }
  // }

  for(int i = 0; i < MAX_PASSWORD_LENGTH; i++){
    if(buffer[i] == master_password[i]){
      continue;
    }
    else{
      Serial.println("Wrong Password");
      return false;
    }
  }
  return true;
}

//Save the password
void set_password()
{
  strcpy(buffer, password);
  strcpy(empty_buffer, buffer);
}

void success_message()
{
  lcd.clear();
  lcd.noCursor();
  lcd.noBlink();
  lcd.setCursor(0, 0);
  lcd.print("Disarm Sequence");
  lcd.setCursor(2, 1);
  lcd.print("Accepted ");
  lcd.write('\2'); //frown
  delay(1000);
}

// Clear the lcd and display the initial message
void reset_lcd()
{
  lcd.clear();
  initial_message();
}

void initial_message()
{
  lcd.blink();

  // set the cursor to the top left
  lcd.setCursor(0, 0);
  lcd.print("Enter passcode");
}

// Displays a message letting the user know the inccorect passcode was used
void bad_passcode_msg()
{
  lcd.clear();
  lcd.noBlink();
  lcd.noCursor();
  lcd.setCursor(0, 0);
  lcd.print("Disarm Sequence");
  lcd.setCursor(2, 1);
  lcd.print("Incorrect ");
  lcd.write(0); // heart
  delay(2000);
}

void set_new_password()
{
  Serial.println("set new password");
  strcpy(buffer, empty_buffer);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Add new password.");
  lcd.setCursor(0, 1);
  lcd.print("Press # to save.");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.blink();
  lcd.print("Enter new pass: ");
  lcd.setCursor(1, 1);

  cursor_x = 0;
  customKeypad.clear();
  customKeypad.tick();
  char key = ' ';
  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    char key = (char)e.bit.KEY;
    if(e.bit.EVENT == KEY_JUST_PRESSED && key != '#'){
      buffer[cursor_x] = key;
      cursor_x++;
      lcd.write(key);
    }
    Serial.println(key);
    if(cursor_x == MAX_PASSWORD_LENGTH || key == '#')
    {
      Serial.print("New password is: ");
      Serial.println(buffer);
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.write("key saved");
      delay(2000);
      reset_code_entered = false;
      break;
    }
  }
  cursor_x = 0;
}
