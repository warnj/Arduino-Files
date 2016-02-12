

int m_InByte              = 0;             // stores incomming serial data
int m_Speed  = 0;
int m_Direction = 0;
int m_currentread = 0;
int m_Pot = 0;
int m_potcontrol = false;


void setup() {
  Serial.begin(9600);  
  pinMode(11, OUTPUT);
}

void loop() {

  m_currentread = analogRead(A0);
  m_Pot = map(m_currentread, 0, 1023, 0, 255);
  analogWrite(11, m_Pot);

   
  Serial.println(m_Pot);

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



