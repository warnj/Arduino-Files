void setup() {}

void loop() {}

void PauseResume(){
  int m_InByte = 0;
  if(Serial.available() > 0){          // If you want to see how many drops have passed and reset the counter, send anything in serial monitor except "r"
    m_InByte = Serial.read();
    if(m_InByte == 'l'){               // if "r" is entered into serial, reset
      Serial.println("Pause");
      bool i = 1;
      while(i == 1){
        if(Serial.available() > 0){          // If you want to see how many drops have passed and reset the counter, send anything in serial monitor except "r"
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


float FmultiMap(float val, float * _in, float * _out, uint8_t size) {
  // take care the value is within range
  // val = constrain(val, _in[0], _in[size-1]);
  if (val <= _in[0]) return _out[0];
  if (val >= _in[size-1]) return _out[size-1];

  // search right interval
  uint8_t pos = 1;    // _in[0] allready tested
  while(val > _in[pos]) pos++;
  
  if (val == _in[pos]) return _out[pos];            // this will handle all exact "points" in the _in array
  
  return (val - _in[pos-1]) * (_out[pos] - _out[pos-1]) / (_in[pos] - _in[pos-1]) + _out[pos-1];           // interpolate in the right segment for the rest
}


void SerialMenu(){
  int m_InByte = 0;
  if(Serial.available() > 0){      
      m_InByte = Serial.read();
      if(m_InByte == 'r'){}
      else if(m_InByte == 'a'){}
      else if(m_InByte == '0'){}
      else if(m_InByte == '1'){}
      else if(m_InByte == '2'){}
      else if(m_InByte == '3'){}
      else if(m_InByte == '4'){}
      else if(m_InByte == '5'){}
      else{Serial.println("Not a Valid Command");}
  }
}


void SoftwareReset() {              // Restarts program from beginning but does not reset the peripherals and registers
  Serial.println("Reset");
  delay(100);                       // Needs time for serial monitor to print characters
  asm volatile ("  jmp 0");  
} 


void PurgeSerialInput(void){ while(Serial.available())  { int m_inByte = Serial.read(); }}

void ClearSerialBuffer(){for (int i = 0; i != BUFFER_SIZE; i++)    { m_SerialPortBuffer[i] = 0;}     }


void LEDMessage(int NumberOfFlashes){
  for(int i = 0; i != NumberOfFlashes; i++)    {
    digitalWrite(13, HIGH);
    delay(400);             
    digitalWrite(13, LOW); 
    delay(400);            
  }    
}


void ledblink(int times, int lengthms, int pinnum){
  for (int x=0; x<times; x++){
    digitalWrite(pinnum, HIGH);
    delay (lengthms);
    digitalWrite(pinnum, LOW);
    delay(lengthms);
  }
}


void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000/frequency/2;
  long numCycles = frequency * length/ 1000;
  for (long i=0; i < numCycles; i++){ 
    digitalWrite(targetPin,HIGH);
    delayMicroseconds(delayValue); 
    digitalWrite(targetPin,LOW);
    delayMicroseconds(delayValue); 
  }
}

/*Checking Free RAM
The ATmega328 has 32K of program memory but only 2K of SRAM. Program memory is for code and RAM is for
dynamic variables. Actually it’s effectively less than 2K of RAM, because the Arduino libraries will
take up some dynamic memory space for themselves.
The Arduino environment will happily let you compile a program that exceeds the micro-controller’s
SRAM limits, however program behavior will become totally unpredictable, the code will do bizarre
things and/or crash. This function will return the number of bytes currently free in SRAM.
*/
int memoryTest() {
  int byteCounter = 0;
  byte *byteArray; 
  while ( (byteArray = (byte*) malloc (byteCounter * sizeof(byte))) != NULL ) {
    byteCounter++; 
    free(byteArray); 
  }
  free(byteArray); 
  return byteCounter;
}





