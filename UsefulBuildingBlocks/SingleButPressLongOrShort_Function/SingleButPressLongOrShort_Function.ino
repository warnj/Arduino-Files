// This program uses a void function to modify a global variable corresponding to when a button is pressed for a short or a long duration

const int Pin = 11;  // the number of the input pin (for me a push button)
const int ledPin = 13;
#define LONG 2
#define SHORT 1
int button = 0;

void setup() {
    Serial.begin(9600);    // Use serial for debugging
    pinMode(ledPin, OUTPUT);
    pinMode(Pin, INPUT_PULLUP); //turn on 20k pullup resistors to simplify switch input
    pinMode(9, INPUT_PULLUP);
}

void loop() {
    SingleButPressShortOrLong(Pin, 2);
    if(button!=0){
        if (button == SHORT){Serial.println("Button Short");}
        if (button == LONG){Serial.println("Button Long");}
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

void SingleButPressShortOrLong(int inputPin, int duration){
    while(button ==0){        
        static int current;    //Current state of the button (LOW is pressed b/c i'm using the pullup resistors
        static int count;      // How long the button was held (secs)
        static byte previous = HIGH;
        static unsigned long firstTime = 0;       // how long since the button was first pressed
        current = digitalRead(inputPin);
        if (current == LOW && previous == HIGH && millis()- firstTime > 200){ firstTime = millis();}    // if the buttons becomes press remember the time
        if (current == LOW && ((millis() - firstTime) % 1000) < 20 && millis() - firstTime > 500){  // within 20ms of one sec interval
            ledblink(1, 50, ledPin); // Each second the button is held blink the indicator led and
            count++;                // and 1 to the counter
        }
        if (current == HIGH && previous == LOW && count >= duration ){button = LONG;}
        if (current == HIGH && previous == LOW && count >=0 && count < duration) {button = SHORT;}
        if (current == HIGH){count = 0;} // reset the counter if the button is not pressed
        previous = current;
        delay(20);
    }
}


