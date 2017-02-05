
#define irLED 12 
#define statusLED 13

unsigned long ledtime = 0;

void setup() {
  pinMode(irLED, OUTPUT);
  pinMode(statusLED, OUTPUT);
  Serial.begin(9600);
  
}

void loop() { 
  int t = 0;
  if (Serial.available() > 0) {
    t = Serial.read();
    digitalWrite(statusLED, HIGH);                      //Timelapse active
    ledtime = millis();
    sendInfraredSignal();                               // to take photo normally - send one IR signal
    }                                                   // in bulb mode one IR signal opens shutter, one more IR signal closes it
  if(millis() - ledtime > 200){
    digitalWrite(statusLED, LOW);
    }
  
}

void sendInfraredSignal() {
  for(int i=0; i<16; i++) {         // 16 very rapid flashes
    digitalWrite(irLED, HIGH);
    delayMicroseconds(11);
    digitalWrite(irLED, LOW);
    delayMicroseconds(11);
   } 
   delayMicroseconds(7330);         // longer delay off
   for(int i=0; i<16; i++) {        // 16 more very rapid flashes
     digitalWrite(irLED, HIGH);
     delayMicroseconds(11);
     digitalWrite(irLED, LOW);
     delayMicroseconds(11);
   }   
}

