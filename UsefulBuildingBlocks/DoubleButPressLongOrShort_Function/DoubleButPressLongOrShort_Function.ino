// This program uses a void function to modify a global variable corresponding to when button two is pressed for a short (sets variable to 1) or a long (2) duration. When buttons two is pressed likewise, the variable is set to 3 or 4.

#define SHORT1 1
#define LONG1 2
#define SHORT2 3
#define LONG2 4
int button = 0;

void setup() {
    Serial.begin(9600);    // Use serial for debugging
    pinMode(13, OUTPUT);
    pinMode(11, INPUT_PULLUP); //turn on 20k pullup resistors to simplify switch input
    pinMode(9, INPUT_PULLUP);
}

void loop() {
    DoubleButPressShortOrLong(11, 9, 2);
    if(button!=0){
        if (button == SHORT1){Serial.println("Button1 Short");}
        if (button == LONG1){Serial.println("Button1 Long");}
        if (button == SHORT2){Serial.println("Button2 Short");}
        if (button == LONG2){Serial.println("Button2 Long");}
        button = 0;
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

void DoubleButPressShortOrLong(int inputPin, int inputPin2, int duration){
    while(button ==0){
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
        if (current == HIGH && previous == LOW && count >= duration ){button = LONG1;}
        if (current == HIGH && previous == LOW && count >=0 && count < duration) {button = SHORT1;}
        if (current == HIGH){count = 0;} // reset the counter if the button is not pressed
        previous = current;
        
        current2 = digitalRead(inputPin2);
        if (current2 == LOW && previous2 == HIGH && millis()- firstTime2 > 200){ firstTime2 = millis();}    // if the buttons becomes press remember the time
        if (current2 == LOW && ((millis() - firstTime2) % 1000) < 20 && millis() - firstTime2 > 500){  // within 20ms of one sec interval
            ledblink(1, 50, 13); // Each second the button is held blink the indicator led and
            count2++;                // and 1 to the counter
        }
        if (current2 == HIGH && previous2 == LOW && count2 >= duration ){button = LONG2;}
        if (current2 == HIGH && previous2 == LOW && count2 >=0 && count2 < duration) {button = SHORT2;}
        if (current2 == HIGH){count2 = 0;} // reset the counter if the button is not pressed
        previous2 = current2;
        
        delay(20);
    }
}


