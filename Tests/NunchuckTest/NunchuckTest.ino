#include <Wire.h>
#include <ArduinoNunchuk.h>

ArduinoNunchuk nun;



void setup() {
  Serial.begin(9600);
  nun.init();
}

void loop() {
  nun.update();

  // Joystick
  //Serial.println(nun.analogX);   // 128middle,27left lowest,219right highest.  x direction, 255 is disconnected, these dont change with 3.3v or 5v
  Serial.println(nun.analogY);   // 133middle,31bottom lowest,229top highest.

  //aileron
  //Serial.print(nun.accelX);// as if held in hand: accel left goes higher, accel right goes lower. straight & level is about 520. rotation 90deg with joystick pointing to left is about 320, then #s increase to 520 again as continue rotation. Max of 720 to right
  //Serial.print("     ");
  //elevator
  //Serial.print(nun.accelY);// highest points stright down at about 730, lowest points up at about 318, normal in hand is about 420.  accel up #s go higher first (then lower as slow downand accel changes directions). accel down #s go lower first.
  //Serial.print("     ");
  //elevator - basically accelY, rotated 90deg
  //Serial.println(nun.accelZ);// highest points forward at about 720, lowest points backward at about 290
  // there is no rudder axis sensor
  
  //Serial.println(nun.zButton);     // 0 normally, 1 when pressed
  //Serial.println(nun.cButton);     // 0 normally, 1 when pressed


  PauseResume();

      
  //delay(30);
 
}





void PauseResume(){
  int m_InByte = 0;
  if(Serial.available()){          // If you want to see how many drops have passed and reset the counter, send anything in serial monitor except "r"
    m_InByte = Serial.read();
    if(m_InByte == 'l'){               // if "r" is entered into serial, reset
      Serial.println("Pause");
      bool i = 1;
      while(i == 1){
        if(Serial.available()){          // If you want to see how many drops have passed and reset the counter, send anything in serial monitor except "r"
          m_InByte = Serial.read();
          if(m_InByte == 'r'){   
             Serial.println("Resume");    
             i = 0;
          }
        }
      }
    }
  }
}

