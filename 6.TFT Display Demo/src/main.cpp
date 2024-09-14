#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ST7789.h>

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

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC,TFT_MOSI,TFT_SCK, TFT_RST);
int deltaSize=0,delta=1;

void setup() {
  Serial.begin(115200);
  tft.init(TFT_HEIGHT, TFT_WIDTH);
  tft.invertDisplay(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(0, 0);
  tft.println("Hello, world!");
  tft.setTextSize(4);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("Hello, world!");
  
}

void loop() {
  tft.drawRect(TFT_HEIGHT/4-deltaSize, TFT_WIDTH/4-deltaSize, TFT_HEIGHT/4+deltaSize, TFT_WIDTH/4+deltaSize, ST77XX_BLACK);
  deltaSize+=delta;
  tft.drawRect(TFT_HEIGHT/4-deltaSize, TFT_WIDTH/4-deltaSize, TFT_HEIGHT/4+deltaSize, TFT_WIDTH/4+deltaSize, ST77XX_RED);  
  delay(100);
   Serial.print("deltaSize: ");
    Serial.println(deltaSize);
  if(deltaSize>=10  || deltaSize<=0){
    delta=-delta;
   
    Serial.print("delta: ");
    Serial.println(delta);
  }
}

