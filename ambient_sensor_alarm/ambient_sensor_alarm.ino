// C++ code
//
const int dark = 130; //set dark parameters
const int sound = 2093; //set noise to play

const int SENSOR = A0; //emitter , collector connected to GND through 10k resistor
const int BUZZER = 8;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT); 
  pinMode(SENSOR, INPUT); 
  Serial.begin(9600);
}

void loop()
{
  int light = analogRead(SENSOR); 
  if (light < dark) { 
    Serial.print(light); 
    Serial.println(" It's dark");
    noTone(8);
  } 
  else { 
    Serial.print(light); 
    Serial.println(" It's light");
    for(int freq = 100; freq < 1010; freq+=10){
       Serial.println(freq);
       tone(BUZZER, freq);
    }
   	// tone(BUZZER, sound);

	} 
  delay(1000);
}
