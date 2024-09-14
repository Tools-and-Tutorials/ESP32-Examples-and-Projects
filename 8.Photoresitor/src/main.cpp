#include <Arduino.h>

#define PR1_PIN 15  //Must be ADC PIN
#define BUZZ_PIN 4  //Must be PWM PIN


int prevValue=0;

bool checkChanges(int[]);
void printValues(int[]);

void setup() {
  Serial.begin(115200);
  pinMode(PR1_PIN, INPUT);
  pinMode(BUZZ_PIN, OUTPUT);
}

//Produce short beep sound on light intensity change and long beep on no light
//In order to reduce the variance ignore last digit of photoresistor value.
void loop() {
  int currentValue = 0; 
  currentValue= analogRead(PR1_PIN);
  if(currentValue/10!=prevValue)
  {
    prevValue=currentValue/10;
    Serial.printf("PR1: %d\n", currentValue/10);
    if(currentValue>0){
      noTone(BUZZ_PIN);
      tone(BUZZ_PIN, 1000, 20); //1000 frequency short beep
    }
    else{
      noTone(BUZZ_PIN);
      tone(BUZZ_PIN, 1000, 500); //1000 frequency long beep
    }
  }
  delay(100);
}
