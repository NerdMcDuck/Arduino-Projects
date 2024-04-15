// C++ code

// Define all the pins being used

#define SW_1 2
#define SW_2 4
#define SW_3 6
#define SW_4 8
#define SW_5 10
#define TARGET 12

const int numOfSwitches = 4;
const int switches[numOfSwitches] = {SW_1, SW_2, SW_3, SW_4};
int passcode[numOfSwitches] = {1,0,0,1};
int passcode_entered[numOfSwitches] = {0,0,0,0};
bool correct_pascode_entered;

// define functions
void get_switch_state();
void verify_entered_passcode();

void setup()
{
  Serial.begin(9600);
  // set switches as input
  for(int i =0; i < numOfSwitches; i++){
    pinMode(switches[i], INPUT);
  }
  
  pinMode(TARGET, OUTPUT);
  
  correct_pascode_entered = false;
 
}

void loop()
{
  while(!correct_pascode_entered){
    get_switch_state();
    verify_entered_passcode();
    delay(500);
    // print the entered passcode
   // for(int i =0; i < numOfSwitches; i++){
   //   Serial.print(passcode_entered[i]);
   // }
   // Serial.println();
   // delay(1000);
  }
  digitalWrite(TARGET, HIGH);
}
// reads the current state of the switches
void get_switch_state(){
  int sw_state;
  
  for(int i = 0; i<numOfSwitches; i++){
    sw_state = digitalRead(switches[i]);
    delay(100);
    passcode_entered[i] = sw_state;
    delay(100);
  }
}

// Checks whether the switches are in the correct configuration
void verify_entered_passcode(){
  for(int i =0; i < numOfSwitches; i++){
    if(passcode_entered[i] != passcode[i]){
     // Serial.println((String)passcode[i]+" "+passcode_entered[i]);
      Serial.println("Incorrect Passcode");
      return;
    }
  }
  Serial.println("Correct");
  correct_pascode_entered = true;
}