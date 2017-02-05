/*
MOSFET should only switch DC current!!!!!!!!!!!!!!!!!!

why is led dimmer when current runs through mosfet?

http://bildr.org/2012/03/rfp30n06le-arduino/


adc reads 769 at 3.8250 V


STOP AT ADC raw OF 845 (850 is about 4.23 V per cell)


*/

#define READ_PIN A0
#define MOSFET_PIN 2
// arduino ground need to be connected to the ground on battery?
// readpin connectted to the first cell of the battery
int writeval = 0;

void setup() {
    Serial.begin(9600);
    pinMode(MOSFET_PIN, OUTPUT);
    digitalWrite(MOSFET_PIN, 1); 
}



void loop() {


    int reading = analogRead(READ_PIN); // 5v is broken into 1024 values
    Serial.println(reading);

    if(Serial.available() > 0){
    while(Serial.available()){
      Serial.read();
    }
    writeval = !writeval;
    digitalWrite(MOSFET_PIN, writeval);
    Serial.print("Writiting val ");
    Serial.println(writeval);
 }
    
    if(reading >= 845){ // 1024 / 5 * 4.2 V = 860 as the 4.2v cutoff
        digitalWrite(MOSFET_PIN, 0); // turn off charger
    }
    
    delay(2000);

}
