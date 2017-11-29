#include <Wire.h>
#include "LiquidCrystal_I2C.h"

#define upButton 2
#define downButton 3
#define okButton 4
#define returnButton 5
const unsigned int tMax = 300U;//valoarea maxima la care se poate seta temperatura

enum {mainMenu, nextOptionMainMenu, afisParam, setParam, setTemp1, setTemp2} stateMachine = mainMenu;
enum {PRESS, UNPRESS, BOTH} buttonState = UNPRESS;
enum {MAIN, SUBTEMPSET} displayMenu = MAIN;

unsigned int buttonPin[4] = {2, 3, 4, 5};
unsigned int buttonStateArray[4] = {100, 100, 100, 100}; //initial values, dummy values;  0 = apasat

/*
#define PRESS 0
#define UNPRESS 1
*/

unsigned int pressingTime = 200;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

char meniu[2][16] = {
  "Display params ", 
  "Set temperature"
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
pinMode(4, INPUT_PULLUP);
pinMode(5, INPUT_PULLUP);
}

unsigned long buttonTimer, bothButtonTimer;
int menuPosition = 0;
void loop() {
  //static unsigned int upButtonState = 1;
  //static unsigned int downButtonState = 1;
  
  readButtons();
  displayLCD();
  //buton UP
  
}//loop

void readButtons(){
  for(int i = 0; i < 4; i++){
    if((digitalRead(buttonPin[i]) == 0) && (buttonStateArray[i] == 100)){
    buttonTimer = millis();
    buttonStateArray[i] = 0;
    //este posibil ca urmatoarea linie sa fie nenecesara
    //butonStateArray[i] = 0; //buton apasat 
  }//if
  }//for

for(int i = 0; i < 4; i++){
  if((digitalRead(buttonPin[i]) == 0) && (millis() - buttonTimer > pressingTime) && (buttonStateArray[i] == 0)){
    buttonTimer = millis(); 
    updateState(buttonPin[i]);
 }
}

for(int i = 0; i < 4; i++){
  if((digitalRead(buttonPin[i]) == 1) && (buttonStateArray[i] == 0)){
    buttonStateArray[i] = 100;
  }
}

 
}//readButtons function



void displayLCD(){
 //afisare LCD
  //meniu principal, submeniu setare temperatura

  //de gandit cum se va afisa daca temperatura este mai ica decat 100, deci doar doua caractere sau poate doar unul
  
  if(stateMachine == mainMenu){
   // lcd.clear();
  lcd.setCursor(0,0);
  if(menuPosition == 0){
  lcd.print(">");
  lcd.print(meniu[0]);
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.print(meniu[1]);
  }
  else {
    lcd.setCursor(0,0);
    lcd.print(" ");
  lcd.print(meniu[0]);
    lcd.setCursor(0,1);
    lcd.print(">");
    lcd.print(meniu[1]);
  }
  }//stateMachine = mainMenu
  
  else if(stateMachine == afisParam){
    
    lcd.setCursor(0, 0);
    lcd.print("1: ");
    lcd.print(temp1);
    lcd.print("  2: ");
    lcd.print(temp2);
    lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print("                ");
  }

  else if(stateMachine == setTemp1){
    lcd.setCursor(0, 0);
    lcd.print("[1: ");
    lcd.print(temp1);
    lcd.print("] 2: ");
    lcd.print(temp2);
    lcd.setCursor(0,1);
    lcd.print("                ");
  }

  else if(stateMachine == setTemp2){
    lcd.setCursor(0, 0);
    lcd.print(" 1: ");
    lcd.print(temp1);
    lcd.print(" [2: ");
    lcd.print(temp2);
    lcd.print("]");
    lcd.setCursor(0,1);
    lcd.print("                ");
  }
  
}//displayLCD function

void updateState(unsigned int p){
  switch(p){
    
    case 2: //-
      if(stateMachine == mainMenu){
        menuPosition--;
        if(menuPosition < 0)
          menuPosition = 0;
      }
      if(stateMachine == setTemp1){
        temp1--;
        if(temp1 < 0)
          temp1 = 0;
      }
      if(stateMachine == setTemp2){
        temp2--;
        if(temp2 < 0)
          temp2 = 0;
      }
      break;

      
     case 3: //+
      if(stateMachine == mainMenu){
        menuPosition++;
        if(menuPosition > 1)
          menuPosition = 1;
      }
      
      
      if(stateMachine == setTemp1){
        temp1++;
        if(temp1 > tMax)
          temp1 = tMax;
      }
      if(stateMachine == setTemp2){
        temp2++;
        if(temp2 > tMax)
          temp2 = tMax;
      }
      break;


      case 4: //OK
        if((stateMachine == mainMenu) && (menuPosition == 0)){
          stateMachine = afisParam;
        }
        else if((stateMachine == mainMenu) && (menuPosition == 1)){
          stateMachine = setTemp1;
        }
        //trebuie vazut daca nu cumva aici trece prin toate starile
        else if(stateMachine == setTemp1){
          stateMachine = setTemp2;
        }
         else if(stateMachine == setTemp2){
          stateMachine = mainMenu;
         }
        
        break;

        case 5: //return
          if((stateMachine == afisParam) || (stateMachine == setTemp1) || (stateMachine == setTemp2))
            stateMachine = mainMenu;
        break;
  }//switch

}

