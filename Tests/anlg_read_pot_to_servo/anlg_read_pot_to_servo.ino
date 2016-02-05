#include <Servo.h>

Servo servo;
const int potpin = 1;
const int servopin = 8;

void setup() {
  //Serial.begin(9600);
  servo.attach(servopin);   
} 

void loop() {
  int potVal = analogRead(potpin);
  //Serial.println(potVal);

  int servoVal = map(potVal, 0, 1024, 0, 180);
  servoVal = constrain(servoVal, 10, 170);
  
  servo.write(servoVal); 
  //Serial.println(servoVal);
}

// windows command prompt ipconfig
//   173.250.146.145
