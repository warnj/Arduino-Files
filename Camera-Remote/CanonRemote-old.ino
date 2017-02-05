/*
      Starts at menu:
        top but to incr # shots 
        top but long press to start taking photots
        bottom but short press to decr # shots
        bottom but long press to change menu selection
     Add menu item for no bulb
     add menu item to change changeminperShot
 */







#define irLED 12 
#define statusLED 13

#define UpBut 11
#define DownBut 10

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 5, 6, 7, 8);       // RS pin, E pin, D4 pin, D5 pin, D6 pin, D7 pin

unsigned long ledtime = 0;
bool firsttimeinmenu = true;
bool entermenu = true;
int menu = 0;

int current;    //Current state of the button (LOW is pressed b/c i'm using the pullup resistors
int count;      // How long the button was held (secs)
byte previous = HIGH;
unsigned long firstTime = 0;       // how long since the button was first pressed 
unsigned long shortpressTime = 0;

int current2;    //Current state of the button (LOW is pressed b/c i'm using the pullup resistors
int count2;      // How long the button was held (secs)
byte previous2 = HIGH;
unsigned long firstTime2 = 0;       // how long since the button was first pressed 
unsigned long shortpressTime2 = 0;

int numShots = 200;
int lastnumShots = 1;
float minperShot = 1;
float lastminperShot = 1;
float changeminperShot = 0;    // how much to change the exposure time on subsequent exposures
float lastchminperShot = 0;

void setup() {
  pinMode(irLED, OUTPUT);
  pinMode(statusLED, OUTPUT);
  pinMode(UpBut, INPUT_PULLUP); //turn on 20k pullup resistors to simplify switch input
  pinMode(DownBut, INPUT_PULLUP); //turn on 20k pullup resistors to simplify switch input
  lcd.begin(16, 2);   // set up the LCD's number of columns and rows:  
}

void loop() { 

  while(entermenu == true && menu == 0){           // change # shots
      if (firsttimeinmenu == true){lcd.clear(); firsttimeinmenu = false; flash();}
      lcd.home();
      lcd.print("#Shots");
      
      lcd.setCursor(7, 0);
      lcd.print(numShots);
      
      lcd.setCursor(0, 1);
      lcd.print("Minutes");      //minutes per shot
      
      lcd.setCursor(8, 1);
      lcd.print(minperShot);
      
      current = digitalRead(UpBut);
      if (current == LOW && previous == HIGH && millis()- firstTime > 200){ firstTime = millis();}    // if the buttons becomes press remember the time 
      if (current == LOW && ((millis() - firstTime) % 1000) < 20 && millis() - firstTime > 500){
        ledblink(1, 50, statusLED); // Each second the button is held blink the indicator led and
        count++;                // and 1 to the counter
        }  
      if (current == HIGH && previous == LOW && count >=2 ){ entermenu = false;}            // long press longer than 2 sec
      if (current == HIGH && previous == LOW && count >=0 && count < 2 && millis() - shortpressTime > 50){     // short press
        numShots++;
        shortpressTime = millis();    
      }                                // with diffent time parameters
      if (current == HIGH){count = 0;} // reset the counter if the button is not pressed
      previous = current; 
      
      current2 = digitalRead(DownBut);
      if (current2 == LOW && previous2 == HIGH && millis()- firstTime2 > 200){ firstTime2 = millis();}    // if the buttons becomes press remember the time 
      if (current2 == LOW && ((millis() - firstTime2) % 1000) < 20 && millis() - firstTime2 > 500){
        ledblink(1, 50, statusLED); // Each second the button is held blink the indicator led and
        count2++;                // and 1 to the counter
      }  
      if (current2 == HIGH && previous2 == LOW && count2 >=2 ){menu = 1;}            // long press longer than 2 sec
      if (current2 == HIGH && previous2 == LOW && count2 >=0 && count2 < 2 && millis() - shortpressTime2 > 50){     // short press
        numShots--;
        shortpressTime2 = millis();      }                                // with diffent time parameters
      if (current2 == HIGH){count2 = 0;} // reset the counter if the button is not pressed
      previous2 = current2;

      if (numShots < 0){numShots = 0;}
      
      if (lastnumShots==10&&numShots==9||lastnumShots==-10&&numShots==-9||lastnumShots==100&&numShots==99||lastnumShots==-100&&numShots==-99){lcd.clear();}
      
      lastnumShots = numShots;
       
  }
  if(firsttimeinmenu == false){firsttimeinmenu=true;}
  
  while(entermenu == true && menu==1){           // change minutes per shot
      if (firsttimeinmenu == true){lcd.clear(); firsttimeinmenu = false; flash();}      
      lcd.home();
      lcd.print("#Shots");
      
      lcd.setCursor(7, 0);
      lcd.print(numShots);
      
      lcd.setCursor(0, 1);
      lcd.print("Minutes");      //minutes per shot
      
      lcd.setCursor(8, 1);
      lcd.print(minperShot);
      
      current = digitalRead(UpBut);
      if (current == LOW && previous == HIGH && millis()- firstTime > 200){ firstTime = millis();}    // if the buttons becomes press remember the time 
      if (current == LOW && ((millis() - firstTime) % 1000) < 20 && millis() - firstTime > 500){
        ledblink(1, 50, statusLED); // Each second the button is held blink the indicator led and
        count++;                // and 1 to the counter
        }  
      if (current == HIGH && previous == LOW && count >=2 ){ entermenu = false;}            // long press longer than 2 sec
      if (current == HIGH && previous == LOW && count >=0 && count < 2 && millis() - shortpressTime > 50){     // short press
        minperShot++;
        shortpressTime = millis();    
      }                                // with diffent time parameters
      if (current == HIGH){count = 0;} // reset the counter if the button is not pressed
      previous = current; 
      
      current2 = digitalRead(DownBut);
      if (current2 == LOW && previous2 == HIGH && millis()- firstTime2 > 200){ firstTime2 = millis();}    // if the buttons becomes press remember the time 
      if (current2 == LOW && ((millis() - firstTime2) % 1000) < 20 && millis() - firstTime2 > 500){
        ledblink(1, 50, statusLED); // Each second the button is held blink the indicator led and
        count2++;                // and 1 to the counter
      }  
      if (current2 == HIGH && previous2 == LOW && count2 >=2 ){menu = 2;}            // long press longer than 2 sec
      if (current2 == HIGH && previous2 == LOW && count2 >=0 && count2 < 2 && millis() - shortpressTime2 > 50){     // short press
        minperShot--;
        shortpressTime2 = millis();      }                                // with diffent time parameters
      if (current2 == HIGH){count2 = 0;} // reset the counter if the button is not pressed
      previous2 = current2;
    
      if (minperShot < 0){minperShot = 0;}
      
      if (lastminperShot==10&&minperShot==9||lastminperShot==-10&&minperShot==-9||lastminperShot==100&&minperShot==99||lastminperShot==-100&&minperShot==-99){lcd.clear();}
      
      lastminperShot = minperShot;   
  }
  if(firsttimeinmenu == false){firsttimeinmenu=true;}

  while(entermenu == true && menu == 2){           // change minutes per shot
      if (firsttimeinmenu == true){lcd.clear(); firsttimeinmenu = false; flash();}      
      lcd.home();
      lcd.print("Minutes");
      
      lcd.setCursor(8, 0);
      lcd.print(minperShot);
      
      lcd.setCursor(0, 1);
      lcd.print("ChngMin/Sht");      //minutes per shot
      
      lcd.setCursor(12, 1);
      lcd.print(changeminperShot);
      
      current = digitalRead(UpBut);
      if (current == LOW && previous == HIGH && millis()- firstTime > 200){ firstTime = millis();}    // if the buttons becomes press remember the time 
      if (current == LOW && ((millis() - firstTime) % 1000) < 20 && millis() - firstTime > 500){
        ledblink(1, 50, statusLED); // Each second the button is held blink the indicator led and
        count++;                // and 1 to the counter
        }  
      if (current == HIGH && previous == LOW && count >=2 ){ entermenu = false;}            // long press longer than 2 sec
      if (current == HIGH && previous == LOW && count >=0 && count < 2 && millis() - shortpressTime > 50){     // short press
        changeminperShot = changeminperShot + 0.5;
        shortpressTime = millis();    
      }                                // with diffent time parameters
      if (current == HIGH){count = 0;} // reset the counter if the button is not pressed
      previous = current; 
      
      current2 = digitalRead(DownBut);
      if (current2 == LOW && previous2 == HIGH && millis()- firstTime2 > 200){ firstTime2 = millis();}    // if the buttons becomes press remember the time 
      if (current2 == LOW && ((millis() - firstTime2) % 1000) < 20 && millis() - firstTime2 > 500){
        ledblink(1, 50, statusLED); // Each second the button is held blink the indicator led and
        count2++;                // and 1 to the counter
      }  
      if (current2 == HIGH && previous2 == LOW && count2 >=2 ){menu = 0;}            // long press longer than 2 sec
      if (current2 == HIGH && previous2 == LOW && count2 >=0 && count2 < 2 && millis() - shortpressTime2 > 50){     // short press
        changeminperShot = changeminperShot - 0.5;
        shortpressTime2 = millis();      }                                // with diffent time parameters
      if (current2 == HIGH){count2 = 0;} // reset the counter if the button is not pressed
      previous2 = current2;
    
      if (minperShot < 0){minperShot = 0;}
      
      if (lastchminperShot==10&&changeminperShot==9||lastchminperShot==-10&&changeminperShot==-9||lastchminperShot==100&&changeminperShot==99||lastchminperShot==-100&&changeminperShot==-99){lcd.clear();}
      
      lastchminperShot = changeminperShot;   
  }
  if(firsttimeinmenu == false){firsttimeinmenu=true;}
  

  while(entermenu == false){
    bool takeshots = false;
    while(takeshots == false){
      if (firsttimeinmenu == true || millis() - firstTime > 300){lcd.clear(); firsttimeinmenu = false;}
      lcd.home();
      lcd.print("Begin?");
      current = digitalRead(UpBut);
      current2 = digitalRead(DownBut);
      if (current == LOW || current2 == LOW){takeshots = true;}
      firstTime = millis();
    }
    
    for (int i=0; i <= numShots; i++) {
      lcd.noDisplay();
      sendInfraredSignal();
      if (i < numShots) delay(minperShot * 60000);    //ms to minutes = 60000
      sendInfraredSignal();                             // dont want this if not in bulb - then need to know length of cam exposure
      minperShot = minperShot + changeminperShot;
      delay (10000);                                    // between shots      
    }
    entermenu = true;
    menu = 0;
    lcd.display();
  }

  if(firsttimeinmenu == false){firsttimeinmenu=true;}



  
}

void ledblink(int times, int lengthms, int pinnum){
  for (int x=0; x<times;x++){
    digitalWrite(pinnum, HIGH);
    delay (lengthms);
    digitalWrite(pinnum, LOW);
    delay(lengthms);
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

void flash(){
  lcd.noDisplay();
  delay(100);
  lcd.display();
  delay(100);
  lcd.noDisplay();
  delay(100);
  lcd.display();  
}