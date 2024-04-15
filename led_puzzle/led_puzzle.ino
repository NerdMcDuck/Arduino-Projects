// C++ code
//
/*
  Push Button Puzzle
*/
// Define all LED and button pins
#define YELLOW_LED 9
#define WHITE_LED 10
#define BLUE_LED 11
#define RED_LED 12
#define GREEN_LED 13

#define YELLOW_BTN 2
#define WHITE_BTN 3
#define BLUE_BTN 4
#define RED_BTN 5
#define GREEN_BTN 6

const int numOfBTNS = 5;
const int patternLen = 10;
// Add them to an array in ascending order
const int LED_PINS[numOfBTNS] = { YELLOW_LED, WHITE_LED, BLUE_LED, RED_LED, GREEN_LED };
const int BTN_PINS[numOfBTNS] = { YELLOW_BTN, WHITE_BTN, BLUE_BTN, RED_BTN, GREEN_BTN };


// Define all methods
void blink_leds_once(unsigned long);
void turn_on_led_on_success();
void blink_failure();
void choose_sequence();
void start_seq();
void play_sequence();
void reset_global_inputs();

// define all global variables
int btn_seq[patternLen] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };    // The code the users will need to input
int input_seq[patternLen] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // The code the users gave

int chosenBTN;
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin

unsigned long startTime = 0;    // when we started holding the buttons
unsigned long holdTime = 2000;  // hold for 2 seconds

bool btn_pressed = false;


void setup() {
  Serial.begin(9600);

  for (int i = 0; i < numOfBTNS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  for (int i = 0; i < numOfBTNS; i++) {
    pinMode(BTN_PINS[i], INPUT);
  }

  blink_leds_once(100);
  delay(500);
  choose_sequence();

  Serial.print("Sequence to enter: ");
  for (int i = 0; i < patternLen; i++) {
    Serial.print(btn_seq[i]);
  }
  Serial.println();
}

void loop() {

  int red_btn_reading = digitalRead(RED_BTN);
  int green_btn_reading = digitalRead(GREEN_BTN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough

  // If both buttons are pressed
  if (red_btn_reading != lastButtonState && green_btn_reading != lastButtonState) {
    btn_pressed = true;
    startTime = millis();
  }
  // Serial.println((String) " " + red_btn_reading + " " + green_btn_reading + " " + btn_pressed + " " + lastButtonState);
  if ((millis() - startTime) >= holdTime && btn_pressed) {
    // if the button state has changed:
    if (red_btn_reading != buttonState && green_btn_reading != buttonState) {
      buttonState = (red_btn_reading & green_btn_reading);

      // start sequence only if the new button state is HIGH
      if (buttonState == HIGH) {
        Serial.println("Buttons have been held for 2 seconds");
        start_seq();
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = (red_btn_reading & green_btn_reading);
}

// This blinks the leds to show status
void blink_leds_once(unsigned long interval) {
  // Port Manipulation is magic: https://docs.arduino.cc/hacking/software/PortManipulation
  for (int i = 0; i < numOfBTNS; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(interval);
    digitalWrite(LED_PINS[i], LOW);
    delay(interval);
  }
  // PORTB = B00111111;  //set pins 8 to 13 to HIGH
  // delay(interval);
  // PORTB = B00000000;  //set pins 8 to 13 to LOW
  // delay(interval);
}

// Turns on all the leds and keeps them on 
void turn_on_led_on_success() {
  Serial.println("Success.");
  for (int i = 0; i < numOfBTNS; i++) {
    digitalWrite(LED_PINS[i], HIGH);
  }
  while(true){
    continue;
  }
}

// Blink just the Red led twice for success
void blink_failure() {
  Serial.println("Failure");
  for (int i = 0; i <= 1; i++) {
    digitalWrite(RED_LED, HIGH);
    delay(500);
    digitalWrite(RED_LED, LOW);
    delay(500);
  }
}

/* 
 * Generate a random number between the valid digital pins
 * this assumes the buttons are ordered ascending without skipping any pins
 */
void choose_sequence() {
  randomSeed(analogRead(A0));
  chosenBTN = random(0, 5);
  int patterns_seq[5][10] = {
    { 2, 2, 3, 4, 5, 6, 5, 4, 3, 2 },  // yellow
    { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },  // white
    { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },  // blue
    { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 },  // red
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 } // green
  };  //green
  Serial.println((String) "chosen seq: " + chosenBTN);
  for (int i = 0; i < patternLen; i++) {
    btn_seq[i] = patterns_seq[chosenBTN][i];

    Serial.print(btn_seq[i]);
  }
  Serial.println();
}
// play the sequence the players need to input
void play_sequence() {
  // blink led sequence
  // int repeats = 0;
  int trueBTN = 0;
  Serial.println((String) "chosenBTN: " + chosenBTN);

  if (chosenBTN == 0) {
    trueBTN = YELLOW_LED;
  } else if (chosenBTN == 1) {
    trueBTN = WHITE_LED;
  } else if (chosenBTN == 2) {
    trueBTN = BLUE_LED;
  } else if (chosenBTN == 3) {
    trueBTN = RED_LED;
  } else {
    trueBTN = GREEN_LED;
  }

  Serial.println((String) "trueBTN: " + trueBTN);
  digitalWrite(trueBTN, HIGH);
  delay(3000);
  digitalWrite(trueBTN, LOW);

  // while (repeats <= 5) {
  //   digitalWrite(trueBTN, HIGH);
  //   delay(1000);
  //   digitalWrite(trueBTN, LOW);
  //   delay(1000);
  //   repeats++;
  // }
  delay(500);
}

// check if the players gave the correct sequence
bool check_button_inputs() {
  for (int i = 0; i < numOfBTNS; i++) {
    Serial.println((String) "i=" + i + " input_seq[i]: " + input_seq[i] + " btn_seq[i]: " + btn_seq[i]);
    if (input_seq[i] != btn_seq[i]) {
      Serial.println("Incorrect input.");
      return false;
    }
  }
  return true;
}

void reset_global_inputs() {
  // set all values back to their initial values
  for (int i = 0; i < numOfBTNS; i++) {
    input_seq[i] = 0;
  }
}

// Starts the button sequence input
void start_seq() {
  delay(800);
  play_sequence();
  Serial.println("Ready to start");
  static bool is_successful = false;
  int btn_presses = 0;
  while (!is_successful) {
    int buttonState;
    for (int i = 0; i < numOfBTNS; i++) {
      buttonState = digitalRead(BTN_PINS[i]);
      delay(50);
      
      while(digitalRead(BTN_PINS[i])){Serial.println("hold");}

      if (btn_presses == patternLen) {
        btn_presses = 0;
        is_successful = check_button_inputs();
        if (is_successful) {
          delay(500);
          turn_on_led_on_success();
          break;
        } else {
          delay(500);
          blink_failure();
          reset_global_inputs();
          delay(1000);
        }
      }

      switch (buttonState) {
        case HIGH:
          input_seq[btn_presses] = BTN_PINS[i];
          btn_presses++;
          digitalWrite(LED_PINS[i], HIGH);
          delay(80);
          Serial.println((String) "i=" + i + " input_seq[btn_presses]: " + input_seq[btn_presses] + " presses: " + btn_presses + " button: " + BTN_PINS[i]);
        default: digitalWrite(LED_PINS[i], LOW);
      }
    }
  }
}