#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

/*
// Konfigurace LCD displeje
LiquidCrystal_I2C lcd(0x27, 20, 4);  // I2C adresa 0x27, 20 sloupců a 4 řádky
const int buttonPin = 3;  // Nahraďte skutečným digitálním pinem pro tlačítko
bool backlightOn = true;  // Počáteční stav podsvícení
*/
// Konfigurace pH měření
const int bufferSize = 300;
float pHBuffer[bufferSize];
int bufferIndex = 0;
float pH;
int relayPin = 8;


void setup() {
  // Inicializace sériového portu pro výstup a Bluetooth
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); 
  Serial.begin(9600);

  // Nastavení pinů pro řízení relé


  /*// Inicializace LCD displeje
  lcd.begin(20, 4);  // Inicializace LCD displeje
  //lcd.backlight();   // Zapnutí podsvícení
  // Print a special character to turn off the backlight
  lcd.createChar(0, B11111111);
  lcd.setCursor(0, 0);
  lcd.write((uint8_t)0);

  pinMode(buttonPin, INPUT_PULLUP);  // Nastavení tlačítka s interním pull-up rezistorem
*/
}

void loop() {
// Read analog voltage from pH sensor
    float voltage = analogRead(A0) * (5.0 / 1024.0);
    // pH calibration points
    //pH4
    float pH4_voltage = 4.238; //pH 4.01 buffer 
    float pH7_voltage = 3.936; //pH 6.86 buffer

    // Calculate slope (m) and y-intercept (b) for the linear equation: pH = m * voltage + b
    float m = (6.86 - 4.01) / (pH7_voltage - pH4_voltage);
    float b = 4.01 - m * pH4_voltage;

    // Calculate pH using the linear equation
    float pH = m * voltage + b;

    Serial.print("pH: ");
    Serial.println(pH, 2);
    //BTSerial.println("pH: " + String(pH, 2));

    // Přidání aktuální hodnoty pH do bufferu
    pHBuffer[bufferIndex] = pH;
    bufferIndex = (bufferIndex + 1) % bufferSize;

    float averagepH = calculateAveragePH();

    if (pH > 6.4 ) {
      digitalWrite(relayPin, LOW);
      Serial.println("Relé: ON");
      //BTSerial.println("Relay: ON");
    } else if (pH <= 6.4) {
      digitalWrite(relayPin, HIGH);
      Serial.println("Relé: OFF");
      //BTSerial.println("Relay: OFF");
    }

    //Serial.print("Průměrné pH za posledních 5 minut: ");
    //Serial.println(averagepH);z

    // Pauza na 1 sekundu
    delay(1000);
  }

/*
// Kontrola tlačítka pro přepínání podsvícení LCD
  if (digitalRead(buttonPin) == LOW) {
  delay(50);  // Debounce delay
  if (digitalRead(buttonPin) == LOW) {
    backlightOn = !backlightOn;  // Toggle the backlight state

    // Turn on or off the backlight based on the state
    if (backlightOn) {
      lcd.setBacklight(LOW);  // Turn on the backlight
    } else {
      lcd.setBacklight(HIGH);  // Turn off the backlight
    }

    // Wait for button release to avoid multiple toggles
    while (digitalRead(buttonPin) == LOW) {
      delay(50);
    }
  }
}
}*/

float calculateAveragePH() {
  float sum = 0;
  for (int i = 0; i < bufferSize; ++i) {
    sum += pHBuffer[i];
  }
  return sum / bufferSize;
}
/*
void displayData(int hour, int minute, float pH, int co2, float temperature) {
  lcd.clear();

  // Format the data as needed
  String timeString = String(hour) + ":" + String(minute);
  String pHStatus = (pH < 6.4) ? "ON" : "OFF";
  String co2Status = (co2 > 6.8) ? "ON" : "OFF";

  // Construct the lines for the 20*4 display
  String line1 = "HH:mm - " + timeString + " | ATF: " + pHStatus;
  String line2 = "pH: " + String(pH, 2) + " | CO2: " + co2Status + " | t: " + String(temperature, 2) + " C";

  // Additional lines if you have a 4-line display
  String line3 = "Additional data line 3";
  String line4 = "Additional data line 4";

  // Print the lines to the LCD
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  lcd.setCursor(0, 2);
  lcd.print(line3);
  lcd.setCursor(0, 3);
  lcd.print(line4);
}
*/