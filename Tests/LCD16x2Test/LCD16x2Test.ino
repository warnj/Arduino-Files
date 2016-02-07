/*
 The circuit:
 * LCD VSS pin to ground
 * LCD VDD pin to 5V
 * 10K pot: ends to +5V and ground, wiper to LCD VO pin (pin 3) - Constrast works OK when VO pin is grounded when supplied by VIN - Works great w/2K to GND when supplied by 5V
 * LCD RS pin to digital pin 12
 * LCD R/W pin to ground
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * D0, D1, D2, & D3 are unconnected
 * LCD A pin to 220ohm resistor and +5V
 * LCD K pin to GND
 */

 

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 5, 6, 7, 8);       // RS pin, E pin, D4 pin, D5 pin, D6 pin, D7 pin

void setup() {  
  lcd.begin(16, 2);   // set up the LCD's number of columns and rows:
  
  lcd.print("Canon Remote V1.0");      // Print a message to the LCD.
  delay(500);
  lcd.clear();
}

void loop() {
  lcd.home();
  lcd.print("Menu");
  lcd.setCursor(0, 1);
  lcd.print("Mode:");               // bulb or not bulb - just make bulb the default for now

  lcd.print("# Shots");

  
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}

