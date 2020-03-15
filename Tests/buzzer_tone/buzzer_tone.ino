int tonevalue = 40;


void setup() {
  pinMode(11, OUTPUT);
}


void loop() {
  tonevalue++;
  if(tonevalue >= 12000) {
    tonevalue = 40;
  }
  tone(11, tonevalue);
  delay(3);
}
