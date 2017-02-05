
#define irLED 11 
#define statusLED 13


int interval = 2;                                    // seconds 
int timelapseDuration = 10;                           // seconds; total duration for all photos
int numberOfShots = timelapseDuration / interval;

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
    for (int i=0; i <= numberOfShots; i++) {
      sendInfraredSignal();
      if (i < numberOfShots) delay(interval * 1000);    //ms
    }
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

