/*
 * MOST RECENT VERSION
 * 
Starts at menu:
top but to incr # shots
top but long press (2sec) to start taking photots
bottom but short press to decr # shots
bottom but long press to change menu selection


unplug the wire going to K on the LCD to turn off backlight
*/


#define irLED 12
#define statusLED 13

#define UpBut 11
#define DownBut 10

#define SHORT1 1
#define LONG1  2
#define SHORT2 3
#define LONG2  4

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 5, 6, 7, 8);       // RS pin, E pin, D4 pin, D5 pin, D6 pin, D7 pin

unsigned long ledtime = 0;
bool firsttimeinmenu = true;
bool entermenu = true;
int menu = 0;
byte buttonState = 0;

int numShots = 900;
int lastnumShots = 900;
float sec_per_shot = 120;
float lastsec_per_shot = 120;
float changesec_per_shot = 0;    // how much to change the exposure time on subsequent exposures
float lastchsec_per_shot = 0;

void setup() {
    pinMode(irLED, OUTPUT);
    pinMode(statusLED, OUTPUT);
    pinMode(UpBut, INPUT_PULLUP); //turn on 20k pullup resistors to simplify switch input
    pinMode(DownBut, INPUT_PULLUP); //turn on 20k pullup resistors to simplify switch input
    lcd.begin(16, 2);   // set up the LCD's number of columns and rows:
}

// main menu loop
void loop() {
    
    while(entermenu == true && menu == 0){           // change # shots
        if (firsttimeinmenu == true){lcd.clear(); firsttimeinmenu = false; flash();}
        lcd.home();
        lcd.print("#Shots");
        
        lcd.setCursor(7, 0);
        lcd.print(numShots);
        
        lcd.setCursor(0, 1);
        lcd.print("Seconds");      //per shot
        
        lcd.setCursor(8, 1);
        lcd.print(sec_per_shot);
        
        buttonState = DoubleButPressShortOrLong(UpBut, DownBut, 2);
        if(buttonState!=0){
            if (buttonState == SHORT1){numShots++;}
            if (buttonState == LONG1){entermenu = false;}
            if (buttonState == SHORT2){numShots--;}
            if (buttonState == LONG2){menu = 1;}
            buttonState = 0;
        }
        
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
        lcd.print("Seconds");      //minutes per shot
        
        lcd.setCursor(8, 1);
        lcd.print(sec_per_shot);
        
        buttonState = DoubleButPressShortOrLong(UpBut, DownBut, 2);
        if(buttonState!=0){
            if (buttonState == SHORT1){
                int incr = (int) (sec_per_shot / 10);
                if(incr < 1) incr = 1;
                sec_per_shot += incr;
            }
            if (buttonState == LONG1){entermenu = false;}
            if (buttonState == SHORT2){
                int incr = (int) (sec_per_shot / 10);
                if(incr < 1) incr = 1;
                sec_per_shot -= incr;
            }
            if (buttonState == LONG2){menu = 2;}
            buttonState = 0;
        }
        
        if (sec_per_shot < 0){sec_per_shot = 0;}
        
        if (lastsec_per_shot==10&&sec_per_shot==9||lastsec_per_shot==-10&&sec_per_shot==-9||lastsec_per_shot==100&&sec_per_shot==99||lastsec_per_shot==-100&&sec_per_shot==-99){lcd.clear();}
        
        lastsec_per_shot = sec_per_shot;
    }
    if(firsttimeinmenu == false){firsttimeinmenu=true;}
    
    while(entermenu == true && menu == 2){           // change minutes per shot
        if (firsttimeinmenu == true){lcd.clear(); firsttimeinmenu = false; flash();}
        lcd.home();
        lcd.print("Seconds");
        
        lcd.setCursor(8, 0);
        lcd.print(sec_per_shot);
        
        lcd.setCursor(0, 1);
        lcd.print("ChngSec/Sht");      //seconds per shot
        
        lcd.setCursor(12, 1);
        lcd.print(changesec_per_shot);
        
        buttonState = DoubleButPressShortOrLong(UpBut, DownBut, 2);
        if(buttonState!=0){
            if (buttonState == SHORT1){changesec_per_shot = changesec_per_shot + 0.5;}
            if (buttonState == LONG1){entermenu = false;}
            if (buttonState == SHORT2){changesec_per_shot = changesec_per_shot - 0.5;}
            if (buttonState == LONG2){menu = 0;}
            buttonState = 0;
        }
        
        if (sec_per_shot < 0){sec_per_shot = 0;}
        
        if (lastchsec_per_shot==10&&changesec_per_shot==9||lastchsec_per_shot==-10&&changesec_per_shot==-9||lastchsec_per_shot==100&&changesec_per_shot==99||lastchsec_per_shot==-100&&changesec_per_shot==-99){lcd.clear();}
        
        lastchsec_per_shot = changesec_per_shot;
    }
    if(firsttimeinmenu == false){firsttimeinmenu=true;}
    

    // Take pics
    while(entermenu == false){   // ready to take pics
        
        bool takeshots = false;
        while(takeshots == false){   // DOUBT WE NEED THIS WHILE STATEMENT
            lcd.clear();
            lcd.home();
            lcd.print("Begin?");
            buttonState = DoubleButPressShortOrLong(UpBut, DownBut, 2);
            if(buttonState!=0){
                if (buttonState == SHORT1){takeshots = true;} // takeshots is more like a way to leave this while loop - use break instead
                if (buttonState == LONG1){menu = 0; entermenu == true; takeshots = true;}
                if (buttonState == SHORT2){takeshots = true;}
                if (buttonState == LONG2){menu = 0; entermenu == true; takeshots = true;}
                buttonState = 0;
            }
        }
        if(entermenu == false) {
            lcd.noDisplay();
            for (int i=0; i <= numShots; i++) {            
                sendInfraredSignal();
                if (i < numShots) delay((sec_per_shot * 1000) + (i * changesec_per_shot * 1000));    //ms to sec = 1000
                sendInfraredSignal();                             // dont want this if not in bulb - then need to know length of cam exposure            
                delay (7000);                                    // 7s delay between shots
            }            
            lcd.display();
        }
        entermenu = true;
        menu = 0;
    }
    
    if(firsttimeinmenu == false){firsttimeinmenu=true;}
}

// blinks given led pinnum for the given number of times with each blink of the given length
void ledblink(int times, int lengthms, int pinnum){
    for (int x=0; x<times;x++){
        digitalWrite(pinnum, HIGH);
        delay (lengthms);
        digitalWrite(pinnum, LOW);
        delay(lengthms);
    }
}

// sends IR remote signal to Canon camera
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

// flashes the lcd display
void flash(){
    lcd.noDisplay();
    delay(100);
    lcd.display();
    delay(100);
    lcd.noDisplay();
    delay(100);
    lcd.display();
}

// returns 1 if button 1 pressed for less than 2 sec, returns 2 if pressed more than 2 seconds
// returns 3 if button 2 pressed for less than 2 sec, returns 4 if pressed more than 2 seconds
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
        if (current == LOW && previous == HIGH && millis() - firstTime > 200) { firstTime = millis(); }    // if the buttons becomes press remember the time
        if (current == LOW && ((millis() - firstTime) % 1000) < 20 && millis() - firstTime > 500){  // within 20ms of one sec interval
            ledblink(1, 50, 13); // Each second the button is held blink the indicator led and
            count++;                // and 1 to the counter
        }
        if (current == HIGH && previous == LOW && count >= duration) {b = LONG1;}                  //LONG
        if (current == HIGH && previous == LOW && count >=0 && count < duration) {b = SHORT1;}      //SHORT
        if (current == HIGH){count = 0;} // reset the counter if the button is not pressed
        previous = current;
        
        current2 = digitalRead(inputPin2);
        if (current2 == LOW && previous2 == HIGH && millis() - firstTime2 > 200) { firstTime2 = millis();}    // if the buttons becomes press remember the time
        if (current2 == LOW && ((millis() - firstTime2) % 1000) < 20 && millis() - firstTime2 > 500){  // within 20ms of one sec interval
            ledblink(1, 50, 13); // Each second the button is held blink the indicator led and
            count2++;                // and 1 to the counter
        }
        if (current2 == HIGH && previous2 == LOW && count2 >= duration) {b = LONG2;}          //LONG
        if (current2 == HIGH && previous2 == LOW && count2 >=0 && count2 < duration) {b = SHORT2;}      //SHORT
        if (current2 == HIGH){count2 = 0;} // reset the counter if the button is not pressed
        previous2 = current2;
        
        if (b != 0){return b;}
        delay(20);
    }
}



