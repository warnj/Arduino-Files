
const int buttonPin = 7;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int mosPin =  2;

int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(mosPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void trigger(int interval, int repeat) {
  for (int i = 0; i < repeat; i++) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(mosPin, HIGH);
    delay(interval);
    digitalWrite(ledPin, LOW);
    digitalWrite(mosPin, LOW);
    delay(interval);
  }
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    trigger(40, 1); //10 right for nickel to nickel, 5 is min
    delay(250);
  }
}
