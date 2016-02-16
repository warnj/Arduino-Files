#define RELAY1  7     
#define RELAY2  8     
#define RELAY3  9     
#define RELAY4  10     


void setup() {
    Serial.begin(9600);
    pinMode(RELAY1, OUTPUT);       
    pinMode(RELAY2, OUTPUT);     
    pinMode(RELAY3, OUTPUT);     
    pinMode(RELAY4, OUTPUT);     
}


void loop() {
   digitalWrite(RELAY1,0);           // Turns ON Relays 1
   digitalWrite(RELAY2,0);  
   digitalWrite(RELAY3,0);  
   digitalWrite(RELAY4,0);  
   Serial.println("Light ON");
   delay(2000);                                      // Wait 2 seconds

   digitalWrite(RELAY1,1);          // Turns Relay Off
   digitalWrite(RELAY2,1);  
   digitalWrite(RELAY3,1);  
   digitalWrite(RELAY4,1);  
   Serial.println("Light OFF");
   delay(2000);   
}
