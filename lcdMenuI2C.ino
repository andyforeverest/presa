#include <Wire.h>
#include "LiquidCrystal_I2C.h"

#define upButton 2
#define downButton 3

enum {PRESS, UNPRESS, BOTH} buttonState = UNPRESS;
enum {MAIN, SUBTEMPSET} displayMenu = MAIN;
/*
#define PRESS 0
#define UNPRESS 1
*/

unsigned int pressingTime = 300;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

char meniu[3][10] = {
  "Dashboard",
  "Set temp ",
  "Set coefs"
  };

int temp1=150, temp2=180;

void setup() {
  // put your setup code here, to run once:
lcd.init();
lcd.init();
  // Print a message to the LCD.
lcd.backlight();
//lcd.print("test");
Serial.begin(9600);
Wire.begin();

pinMode(2, INPUT_PULLUP);
pinMode(3, INPUT_PULLUP);
}

unsigned long upButtonTimer, downButtonTimer, bothButtonTimer;

void loop() {
  //static unsigned int upButtonState = 1;
  //static unsigned int downButtonState = 1;
  static int menuPosition = 0;

  //buton UP
  
  if((digitalRead(upButton) == 0) && (buttonState == UNPRESS) && (digitalRead(downButton) == 1)){
    delay(50);
    upButtonTimer = millis();
    buttonState = PRESS;
  }

  if((digitalRead(upButton) == 0) && (millis() - upButtonTimer > pressingTime) && (buttonState == PRESS) && (digitalRead(downButton) == 1)){
    upButtonTimer = millis();
    //lcd.setCursor(0,1);
    //lcd.print("up  ");
    if(displayMenu == MAIN){
      lcd.clear();
    menuPosition++;
    if(menuPosition > 2)
      menuPosition = 0;
    }
    else
    {
      temp1++;
    }
    Serial.println(temp1);
  }
  
  if((digitalRead(upButton) == 1) && (buttonState == PRESS) && (digitalRead(downButton) == 1)){
    
    buttonState = UNPRESS;
  }

  //buton DOWN

 if((digitalRead(downButton) == 0) && (buttonState == UNPRESS) && (digitalRead(upButton) == 1)){
  delay(50);
    downButtonTimer = millis();
    buttonState = PRESS;
  }

  if((digitalRead(downButton) == 0) && (millis() - downButtonTimer > pressingTime) && (buttonState == PRESS) && (digitalRead(upButton) == 1)){
    downButtonTimer = millis();
    //lcd.setCursor(0,1);
    //lcd.print("down");
    if(displayMenu == MAIN){
      lcd.clear();
    menuPosition--;
    if(menuPosition < 0)
      menuPosition = 2;
    }
    else
    {
      temp1--;
    }
     Serial.println(temp1);
  }
  
  if((digitalRead(downButton) == 1) && (buttonState == PRESS) && (digitalRead(upButton) == 1)){
    
    buttonState = UNPRESS;
  }

  //afisare LCD
  //meniu principal, submeniu setare temperatura
  if(displayMenu == MAIN){
   // lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("> ");
  lcd.print(meniu[menuPosition]);
  lcd.setCursor(0,1);
  if(menuPosition + 1 > 2)
  lcd.print(meniu[0]);
  else
  lcd.print(meniu[menuPosition+1]);
  }

  else{
    
    lcd.setCursor(0, 0);
    lcd.print("1: ");
    lcd.print(temp1);
    lcd.print("  2: ");
    lcd.print(temp2);
  }

  //selectare submeniu setare temperatura
  //
  if((digitalRead(downButton) == 0) && ((buttonState == UNPRESS)) && (digitalRead(upButton) == 0)){
    delay(50);
    bothButtonTimer = millis();
    buttonState = BOTH;
    //upButtonState = PRESS;
  }

  if((digitalRead(downButton) == 0) && (digitalRead(upButton) == 0) && (millis() - bothButtonTimer > pressingTime) && (buttonState == BOTH)){
    bothButtonTimer = millis();
    if(displayMenu == MAIN){
      displayMenu = SUBTEMPSET;
      lcd.clear();
    }
     else{
      displayMenu = MAIN;
      lcd.clear();
     }
      Serial.println(temp1);
  }

  if((digitalRead(downButton) == 1) && (digitalRead(upButton) == 1) && (buttonState == BOTH)){
    buttonState = UNPRESS;
  }
  
}//loop
