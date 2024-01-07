#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

/*
// Create a custom character:
byte fullbottle[8] = {0x0E,0x0E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}; //fullbottle
*/
byte bottleStates[6][8] = {
  {0x0E, 0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, // fullbottle
  {0x0E, 0x0A, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, // 80percentbottle
  {0x0E, 0x0A, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F}, // 60percentbottle
  {0x0E, 0x0A, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x1F}, // 40percentbottle
  {0x0E, 0x0A, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F}, // 20percentbottle
  {0x0E, 0x0A, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F}  // 5percentbottle
};

byte temperature[8] = {0x0E,0x0A,0x0A,0x0A,0x0A,0x11,0x11,0x0E};
byte co2index[8] = {0x00,0x00,0x00,0x0C,0x12,0x04,0x08,0x1E};  
byte fish[8]={0x00,0x16,0x1F,0x16,0x00,0x00,0x00,0x00};
  
void setup() {
  lcd.init();
  lcd.backlight();

  // Create characters directly:
  lcd.createChar(0, bottleStates[0]); //fullbottle
  lcd.createChar(1, bottleStates[3]); //40percent
  lcd.createChar(3, bottleStates[5]); //empty
  lcd.createChar(4, temperature);
  lcd.createChar(5, co2index);
  lcd.createChar(6, fish);
  // Clears the LCD screen:
  lcd.clear();

}

void loop() {
  //#########  NEED TO ADD LOGIC VARIABLES #########
  
  //#### MAIN MENU #####
  lcd.backlight();
  //display();
  //Write header
  lcd.setCursor(4, 0);
  lcd.write(byte(6));
  lcd.setCursor(5,0);
  lcd.print("Aqua controller");

  //first column
  //write CO2
  lcd.setCursor(0, 0);
  lcd.print("CO");
  lcd.setCursor(2, 0);
  lcd.write(byte(5));

  //write CO2 ppm status
  lcd.setCursor(0,1);
  lcd.print(33);
  lcd.setCursor(2,1);
  lcd.print("ppm");

  //write relay on/off
  lcd.setCursor(0,2);
  lcd.print("Rel_");
  lcd.setCursor(4,2);
  lcd.print("Off");

  //write CO2 cartridge status
  lcd.setCursor(0,3);
  lcd.write(byte(1));
  lcd.setCursor(1, 3);
  lcd.print("86%");
  
  //second column
  //write temp status
  lcd.setCursor(8, 1);
  lcd.write(byte(4));
  lcd.setCursor(9, 1);
  lcd.print("25C");
  lcd.setCursor(12, 1);
  lcd.print(char(223));

  //write pH status
  lcd.setCursor(8,2);
  lcd.print("pH");
  lcd.setCursor(10,2);
  lcd.print("6.47");

  //write mode
  lcd.setCursor(8, 3);
  lcd.print("mod_");
  lcd.setCursor(12,3);
  lcd.print("D");
  delay(1000);

  //third column
  lcd.setCursor(15,1);
  lcd.print("22:59");

  //lcd.noBacklight();
  //delay(2000);

}

