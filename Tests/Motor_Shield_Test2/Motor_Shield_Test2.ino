/*
Serial Commands:

0 = set speed 0
1 = set speed 30
2 = set speed 50
3 = set speed 100
4 = set speed 150
5 = set speed 200
6 = set speed 255
a = add 5 to speed
s = subtract 5 from speed
r = reverse directions
p = control motor speed with Pot
m = resume motor control with serial

30 is about the lowest speed turning occurs at
 
 */

#include <AFMotor.h>



int m_InByte              = 0;             // stores incomming serial data
int m_Speed  = 0;
int m_Direction = 0;
int m_currentread = 0;
int m_Pot = 0;
int m_potcontrol = false;

AF_DCMotor motor(3);

void setup() {
  Serial.begin(9600);  
}

void loop() {

  m_currentread = analogRead(A5);
  m_Pot = map(m_currentread, 0, 1023, 0, 255);

  if(m_potcontrol == false){
  motor.setSpeed(m_Speed);
  }
  else{
  motor.setSpeed(m_Pot);
  Serial.println(m_Pot);
  }
  
  if(m_Direction == 0){
  motor.run(FORWARD);
  }
  else{
  motor.run(BACKWARD);
  }

    if(Serial.available() > 0){          // If you want to see how many drops have passed and reset the counter, send anything in serial monitor except "r"
    m_InByte = Serial.read();
    if(m_InByte == '1'){               // if "r" is entered into serial, reset
      Serial.print("Speed =  ");
       m_Speed = 30;
       Serial.println(m_Speed);
       PurgeSerialInput();
      }
    else if(m_InByte == '2'){
      Serial.print("Speed =  ");
      m_Speed = 50;
      Serial.println(m_Speed);
      PurgeSerialInput();
      }
    else if(m_InByte == '3'){
      Serial.print("Speed =  ");
      m_Speed = 100;
      Serial.println(m_Speed);
      PurgeSerialInput();
      }
    else if(m_InByte == '4'){
      Serial.print("Speed =  ");
      m_Speed = 150;
      Serial.println(m_Speed);
      PurgeSerialInput();
      }
    else if(m_InByte == '5'){
      Serial.print("Speed =  ");
      m_Speed = 200;
      Serial.println(m_Speed);
      PurgeSerialInput();
      }
    else if(m_InByte == '6'){
      Serial.print("Speed =  ");
      m_Speed = 255;
      Serial.println(m_Speed);
      PurgeSerialInput();
      }
    else if(m_InByte == '0'){
      Serial.print("Speed =  ");
      m_Speed = 0;
      Serial.println(m_Speed);
      PurgeSerialInput();
      }
    else if(m_InByte == 'a'){
      Serial.print("Speed +5:  ");
      m_Speed = m_Speed + 5;
      Serial.println(m_Speed);
      PurgeSerialInput();
      }
    else if(m_InByte == 's'){
      Serial.print("Speed -5:  ");
      m_Speed = m_Speed - 5;
      Serial.println(m_Speed);
      PurgeSerialInput();
      }
    else if(m_InByte == 'p'){
      Serial.println("Speed   :  Pot Control");
      m_potcontrol = true;
      PurgeSerialInput();
      }
   else if(m_InByte == 'm'){
      Serial.println("Speed   :  Serial Control");
      m_potcontrol = false;
      PurgeSerialInput();
      }
    else if(m_InByte == 'r'){
      Serial.println("Reverse");
      if(m_Direction == 0){m_Direction = 1;}
      else {m_Direction = 0;}
      PurgeSerialInput();
      }
    else {
      Serial.println("Not Valid Command");
      PurgeSerialInput();
      }
      
  } 
 }


 void PurgeSerialInput()            // Removes the values from the serial input buffer
{
 delay(50);         
 while(Serial.available())
  {Serial.read();}
}

/*
void PotControl(){
while(1){
  m_currentread = analogRead(A5);
  m_Pot = map(m_currentread, 0, 1023, 0, 255);
  motor.setSpeed(m_Pot);
  Serial.print("Speed  =  ");
  Serial.println(m_Pot);
  
  if(Serial.available() > 0){          // If you want to see how many drops have passed and reset the counter, send anything in serial monitor except "r"
    m_InByte = Serial.read();
    if(m_InByte == 'm'){               // RESUME serial controls
      Serial.println("Speed  :  Serial Control");
      break;
    }
}
}
}
*/



