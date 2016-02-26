// This program includes a function that returns a value of 1 when button 1 is pressed for less than a set duration of time and a value of 2 when the button is pressed and held for longer than the duration.
// returns 3 or 4 when the second button is pressed in this fashion.

#define SHORT1 1
#define LONG1 2
#define SHORT2 3
#define LONG2 4

void setup() {
    Serial.begin(9600);    // Use serial for debugging
    pinMode(13, OUTPUT);
    pinMode(11, INPUT_PULLUP); //turn on 20k pullup resistors to simplify switch input
    pinMode(9, INPUT_PULLUP);
}

void loop() {
    byte but = DoubleButPressShortOrLong(11, 9, 2);
    if(but!=0){
        if (but == SHORT1){Serial.println("Button1 Short");}
        if (but == LONG1){Serial.println("Button1 Long");}
        if (but == SHORT2){Serial.println("Button2 Short");}
        if (but == LONG2){Serial.println("Button2 Long");}
        but = 0;
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

byte DoubleButPressShortOrLong(int inputPin, int inputPin2, int duration){
    byte b = 0;
    while(b ==0){
        static int current;    //Current state of the button (LOW is pressed b/c i'm using the pullup resistors
        static int count;      // How long the button was held (secs)
        static byte previous = HIGH;
        static unsigned long firstTime = 0;       // how long since the button was first pressed
        
        static int current2;    //Current state of the button (LOW is pressed b/c i'm using the pullup resistors
        static int count2;      // How long the button was held (secs)
        static byte previous2 = HIGH;
        static unsigned long firstTime2 = 0;       // how long since the button was first pressed
        
        current = digitalRead(inputPin);
        if (current == LOW && previous == HIGH && millis()- firstTime > 200){ firstTime = millis();}    // if the buttons becomes press remember the time
        if (current == LOW && ((millis() - firstTime) % 1000) < 20 && millis() - firstTime > 500){  // within 20ms of one sec interval
            ledblink(1, 50, 13); // Each second the button is held blink the indicator led and
            count++;                // and 1 to the counter
        }
        if (current == HIGH && previous == LOW && count >= duration ){b = LONG1;}                  //LONG
        if (current == HIGH && previous == LOW && count >=0 && count < duration) {b = SHORT1;}      //SHORT
        if (current == HIGH){count = 0;} // reset the counter if the button is not pressed
        previous = current;
        
        current2 = digitalRead(inputPin2);
        if (current2 == LOW && previous2 == HIGH && millis()- firstTime2 > 200){ firstTime2 = millis();}    // if the buttons becomes press remember the time
        if (current2 == LOW && ((millis() - firstTime2) % 1000) < 20 && millis() - firstTime2 > 500){  // within 20ms of one sec interval
            ledblink(1, 50, 13); // Each second the button is held blink the indicator led and
            count2++;                // and 1 to the counter
        }
        if (current2 == HIGH && previous2 == LOW && count2 >= duration ){b = LONG2;}          //LONG
        if (current2 == HIGH && previous2 == LOW && count2 >=0 && count2 < duration) {b = SHORT2;}      //SHORT
        if (current2 == HIGH){count2 = 0;} // reset the counter if the button is not pressed
        previous2 = current2;
        
        if (b != 0){return b;}
        delay(20);
    }
}


