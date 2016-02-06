#include <Servo.h>

Servo servo;
const int potpin = 0;
const int servopin = 11;

void setup () {
  Serial.begin(9600);
  servo.attach(servopin);   
} 

void loop () {
  int potVal = analogRead(potpin);
  Serial.println(potVal);

  int servoVal = map(potVal, 0, 1024, 0, 180);
  //servoVal = constrain(servoVal, 10, 170);
  
  writeServo(servoVal); 
  //Serial.println(servoVal);
}


// HXT 900 PWM range is between 450 and 2450
void writeServo (int deg) {
    int msVal = map(deg, 0, 180, 450, 2450);
    servo.writeMicroseconds(msVal);    
}

