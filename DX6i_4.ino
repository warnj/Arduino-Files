
// x left(64) right(191)
// y forward(64) backward(191)


void setup(){
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);
}


void loop() {
   int elevator = pulseIn(2, HIGH);
   int aileron = pulseIn(3, HIGH);
   Serial.print(elevator);
   Serial.print("   ");
   Serial.println(aileron);
   int y = map(elevator, 1065, 1905, 0, 100);
   int x = map(aileron, 1065, 1905, 0, 100);
   Serial.print(x);
   Serial.print("   ");
   Serial.println(y);
   delay(10);   
}
