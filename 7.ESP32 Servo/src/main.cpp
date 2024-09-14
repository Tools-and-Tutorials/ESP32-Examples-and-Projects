#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ST7789.h>
#include <ESP32Servo.h>

#define TFT_CS 15
#define TFT_DC 2
#define TFT_RST 4
#define TFT_MOSI 23
#define TFT_SCK 18
#define TFT_MISO 19
// #define TFT_LED +(POWER)
// #define TOUCH_CLK 18    
// #define TOUCH_CS 21     
// #define TOUCH_DIN 23    
// #define TOUCH_DO 19     

#define TFT_WIDTH 320
#define TFT_HEIGHT 240

#define SERVO_PIN 17
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC,TFT_MOSI,TFT_SCK, TFT_RST);
int delta=1,degree=0;
Servo servo;

void initDisplay();
void updateDisplay(int);

void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
  initDisplay();
}

void initDisplay()
{
  tft.init(TFT_HEIGHT, TFT_WIDTH);
  tft.invertDisplay(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("Hi");
  servo.write(degree);
}

void loop() {
  // for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
  //   servo.write(posDegrees);
  //   Serial.println(posDegrees);
  //   updateDisplay();
  // }

  // for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
  //   servo.write(posDegrees);
  //   Serial.println(posDegrees);
  //   updateDisplay();
  // }
  if(degree<=1)
    delta=1;
  else if(degree>=179)
    delta=-1;
  degree+=delta;
  servo.write(degree);
  Serial.println(degree);
  updateDisplay(degree);
}

void updateDisplay(int pos)
{
  tft.setCursor(TFT_WIDTH/2-60, TFT_HEIGHT/2);
  tft.setTextColor(ST77XX_YELLOW,ST77XX_BLACK);
  tft.printf("Degree: %3d",pos);
  delay(2);
}
