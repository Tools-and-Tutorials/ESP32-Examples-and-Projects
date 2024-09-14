/**
 * @file main.cpp
 * @author Jey (jeyan.py@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-09-06
 * 
 * @copyright Copyright (c) 2024
 * 
 * Program: LED Blink
 * Platform: Arduino / ESP32
 * Author: Jey
 * Date: 2024-09-06
 * Time: 7:00
 * IDE: Visual Studio Code
 * GitHub: https://github.com/Tools-and-Tutorials/ESP32-Examples
 * Website: NA
 * Email: jeyan.py@gmail.com
 * License: None
 *
 */
#include <Arduino.h>

const int LED_PIN=5;

void setup() {
  //set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

/**
 * @brief Turn on and off the LED every second
 * @details
 * @param none
 * @return none
 * 
 */
void loop() {
  //turn the LED on
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  //turn the LED off
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
