#include <Arduino.h>

#define PR1_PIN 15
#define PR2_PIN 4

const int numberOfPins=2;
int prevValues[] = {0,0};

bool checkChanges(int[]);
void printValues(int[]);

void setup() {
  Serial.begin(115200);
  pinMode(PR1_PIN, INPUT);
  pinMode(PR2_PIN, INPUT);
}

void loop() {
  int currentValues[] = {0,0}; 
  currentValues[0]= analogRead(PR1_PIN);
  currentValues[1]= analogRead(PR2_PIN);
  if(checkChanges(currentValues))
  {
   
    printValues(currentValues);
  }
  delay(100);
}

//return true if any values changed, false otherwise
bool checkChanges(int *currentValues)
{
  bool changed = false;
  for (size_t i = 0; i < numberOfPins; i++)
  {
    if (currentValues[i]/10 != prevValues[i])
    {
      Serial.printf("PR%d: ", i);
      Serial.println(currentValues[i]);
      prevValues[i] = currentValues[i]/10;
      changed = true;
    }
  }
  return changed;
}

void printValues(int currentValues[])
{
  Serial.printf("Number of items %d",numberOfPins);
  Serial.println();
  for (size_t i = 0; i < numberOfPins; i++)
  {
    Serial.printf("PR%d: %d ",i,currentValues[i]/10);
  }
  Serial.println();
}
