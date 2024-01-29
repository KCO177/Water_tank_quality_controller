#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

int upButton = 10;
int downButton = 11;
int selectButton = 12;
int shutDownButton = 9;
int menu = 1;
int Timer = 0;
int Timer_1 = 0;
int Timer2 = 0;
int Timer3 = 0;


void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  updateMenu();
}

void loop() {
  if (!digitalRead(downButton)){
    menu++;
    updateMenu();
    delay(100);
    while (!digitalRead(downButton));
  }
  if (!digitalRead(upButton)){
    menu--;
    updateMenu();
    delay(100);
    while(!digitalRead(upButton));
  }
  if (!digitalRead(selectButton)){
    executeAction();
    updateMenu();
    delay(100);
    while (!digitalRead(selectButton));
  }
}

void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">Display OFF ");
      lcd.setCursor(0, 1);
      lcd.print(" Timer CO2 ON/OFF");
      lcd.setCursor(0, 2);
      lcd.print(" pH Probe calib");
      lcd.setCursor(0, 3);
      lcd.print(" CO2 Bottleweight");
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Display OFF ");
      lcd.setCursor(0, 1);
      lcd.print(">Timer CO2 ON/OFF");
      lcd.setCursor(0, 2);
      lcd.print(" pH Probe calib");
      lcd.setCursor(0, 3);
      lcd.print(" CO2 Bottleweight");
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Display OFF ");
      lcd.setCursor(0, 1);
      lcd.print(" Timer CO2 ON/OFF");
      lcd.setCursor(0, 2);
      lcd.print(" pH Probe calib");
      lcd.setCursor(0, 3);
      lcd.print(">CO2 Bottleweight");
      break;
  }
}

void executeAction() {
  switch (menu) {
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
    case 3:
      action3();
      break;
    case 4:
      action3();
      break;
  }
}

void action1() {
  lcd.clear();
  lcd.print(">Switch display OFF");
  Timer++;
  if(Timer ==2){
  //some action
  Timer=0;
  }
  else{
    //some action
  }
  delay(1500);
}

void action2() {
  lcd.clear();
  lcd.print(">Set the timer");
    Timer_1++;
  if(Timer_1 ==2){
  Timer_1=0;
  }
  else{
  }
  delay(1500);
}

void action3() {
  lcd.clear();
  lcd.print(">Ph probe");
    Timer2++;
  if(Timer2 ==2){
  Timer2=0;
  }
  else{
    }
  delay(1500);
}

void action4() {
  lcd.clear();
  lcd.print(">Weight calib");
    Timer2++;
  if(Timer2 ==2){
  Timer2=0;
  }
  else{
    }
  delay(1500);
}