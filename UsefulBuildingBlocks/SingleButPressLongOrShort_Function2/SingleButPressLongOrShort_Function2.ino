// This program includes a function that returns a value of 1 when a button is pressed for less than a set duration of time and a value of 2 when the button is pressed and held for longer than the duration

const byte butPin = 11;  // the number of the input pin (for me a push button)
const byte ledPin = 13;

void setup() {
    Serial.begin(9600);    // Use serial for debugging
    pinMode(ledPin, OUTPUT);
    pinMode(butPin, INPUT_PULLUP); //turn on 20k pullup resistors to simplify switch input
}

void loop() {
    byte button = SingleButPressShortOrLong(butPin, 2);
    if(button!=0) {
        if (button == 1){Serial.println("Button Short");}
        if (button == 2){Serial.println("Button Long");}
        button = 0;
    }    
}

void ledblink(int times, int lengthms, int pinnum){
    for (int x=0; x<times; x++) {
        digitalWrite(pinnum, HIGH);
        delay (lengthms);
        digitalWrite(pinnum, LOW);
        delay(lengthms);
    }
}

byte SingleButPressShortOrLong(int inputPin, int duration){
    byte b = 0;
    while (b == 0) {
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
        if (current == HIGH && previous == LOW && count >= duration ){b = 2;}                      // LONG
        if (current == HIGH && previous == LOW && count >=0 && count < duration) {b = 1;}         // SHORT
        if (current == HIGH){count = 0;} // reset the counter if the button is not pressed
        previous = current;
        if (b != 0) {return b;}
        delay(20);
    }
}


