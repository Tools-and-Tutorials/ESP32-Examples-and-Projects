#include <Arduino.h>
#include <time.h>

#define VERBOSE 0
#define MICROSEC_TO_SEC 1000000ULL

// define the states of the traffic light
enum State
{
  WARN, // warning state - blink yellow
  OFF,  // off state - no lights are on until timers runs out (e.g. 30 seconds)
  ON    // on state - all lights are on
};
#define STATE_STRING(t) (t == WARN ? "WARN" : t == OFF ? "OFF" : t == ON ? "ON" : "UNKNOWN")

// define the actions of the traffic light
enum Actions
{
  RED,
  YELLOW,
  GREEN,
  NONE
};
#define ACTION_STRING(t) (t == RED ? "RED" : t == YELLOW ? "YELLOW" : t == GREEN ? "GREEN" : t == NONE ? "NONE" : "UNKNOWN")


// define the times for the traffic light
const int RED_TIME = 5;
const int YELLOW_TIME = 2;
const int GREEN_TIME = 5;

// define the pins for the traffic light
const int RED_PIN = 16;
const int YELLOW_PIN = 17;
const int GREEN_PIN = 18;

// define the pin for the switch
const int BUTTON_PIN = 22;

const int SLEEP_TIME = 30;

unsigned long runTime;   // number of seconds the traffic light has been running
unsigned long startTime; // millis timer for each action performed, i.e. each seconds this is reset to 0
unsigned long last_interrupt_time = 0;
Actions currentAction;
Actions previousAction;
State state = ON;
State previousState = ON;

void On();
void Off();
void Warn();
void TurnOffLights();
void runTimeControll();
void logActions();
void logStausChange();
void checkAndToggleButton();

void setup()
{
  runTime = 0;
  startTime = millis();
  Serial.begin(115200);
  Serial.println("Traffic light starting...");
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), checkAndToggleButton, RISING);
}

void IRAM_ATTR checkAndToggleButton()
{
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200)
  {
    if (state == ON)
    {
      previousState = ON;
      state = WARN;
    }
    else if (state == WARN)
    {
      previousState = WARN;
      state = OFF;
    }
    else
    {
      previousState = OFF;
      state = ON;
    }
  }
  last_interrupt_time = interrupt_time;
}

// the loop function runs over and over again forever
void loop()
{
  logStausChange();
  if (state == ON)
    On();
  else if (state == WARN)
    Warn();
  else
    Off();
}

void On()
{
  if (runTime < RED_TIME)
  {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    currentAction = RED;
  }
  else if (runTime < RED_TIME + YELLOW_TIME)
  {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    currentAction = YELLOW;
  }
  else if (runTime < RED_TIME + YELLOW_TIME + GREEN_TIME)
  {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    currentAction = GREEN;
  }
  else if (runTime < RED_TIME + 2 * YELLOW_TIME + GREEN_TIME)
  {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    currentAction = YELLOW;
  }
  else
  {
    runTime = 0;
  }
  runTimeControll();
}

void Off()
{
  TurnOffLights();
  esp_sleep_enable_timer_wakeup(SLEEP_TIME * MICROSEC_TO_SEC);
  Serial.print("Going to sleep now for ");
  Serial.print(SLEEP_TIME);
  Serial.println(" seconds");
  Serial.flush();
  esp_deep_sleep_start();
  // if(OFF_TIME>0){
  //   runTimeIncrement();
  //   if(runTime>=OFF_TIME){
  //     state=ON;
  //     runTime=0;
  //   }else{
  //     logActions();
  //   }
  // }
}

void Warn()
{
  if (runTime == 0)
  {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    currentAction = NONE;
  }
  else if (runTime == 1)
  {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    currentAction = YELLOW;
  }
  runTimeControll();
}

void TurnOffLights()
{
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  currentAction = NONE;
  runTimeControll();
}

void runTimeControll()
{
  if (millis() - startTime >= 1000)
  {
    switch (state)
    {
    case ON:
      runTime++;
      break;
    case WARN:
      runTime=(runTime==1)?0:1;
      break;
    default:
      runTime=0;
    }
    startTime = millis();
    logActions();
  }
}

void logStausChange()
{
  if (previousState != state)
  {
    Serial.print("State changed from ");
    Serial.print(STATE_STRING(previousState));
    Serial.print(" to ");
    Serial.println(STATE_STRING(state));
    previousState = state;
  }
}

void logActions()
{
  if (currentAction != previousAction)
  {
    if (state == ON && previousAction != currentAction)
    {
      Serial.print("Current action: ");
      Serial.println(ACTION_STRING(currentAction));
      previousAction = currentAction;
#if VERBOSE
      Serial.print("Run time: ");
      Serial.println(runTime);
      Serial.print("State: ");
      Serial.println(state);
      Serial.print("Start time: ");
      Serial.println(startTime);
      Serial.print("Millis: ");
      Serial.println(millis());
#endif
    }
    else if (state == OFF && SLEEP_TIME > 0)
    {
      Serial.print("Time to Turn On: ");
      Serial.println(SLEEP_TIME - runTime);
    }
    else if (state == WARN)
    {
      #if VERBOSE
      Serial.print("Warning:");
      Serial.println(runTime);
      #endif
    }
  }
}
