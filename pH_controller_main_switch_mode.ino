#include <SPI.h>
#include <SD.h>
#include <math.h>
#include "RTClib.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include "HX711.h"

RTC_DS1307 DS1307;

float pH4V; //= 4.238;  
float pH7V; //= 3.936;
int KH; //== 6;
int DayH;
int DayM;
int NightH;
int NightM;
       

const int bufferSize = 100;
float pHBuffer[bufferSize];
int bufferIndex = 0;
int mode = 1;
const int sd_CS = 10;
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;

HX711 scale;

OneWire oneWireDs(7);
DallasTemperature tempSen(&oneWireDs);

bool ATO = false;
int debounceDelay = 3000;  // 3 seconds debounce delay
unsigned long lastDebounceTime = 0;
unsigned long previousTempMillis = 0;
unsigned long lastSecondMillis = 0;
unsigned long previousScaleMillis = 0;


void setup() {
  
  
  pinMode(2, OUTPUT); // Assuming pin 2 is used for ATO
  pinMode(3, OUTPUT); // Assuming pin 3 is used for relay
  digitalWrite(3, HIGH);
  DS1307.adjust(DateTime(F(__DATE__), F(__TIME__)));
  tempSen.begin();
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  char m = ' '; //
  
  //read setup values
  EEPROM.get(0, pH4V);
  EEPROM.get(1, pH7V);
  EEPROM.get(2, KH);
  EEPROM.get(3, DayH);
  EEPROM.get(4, DayM);
  EEPROM.get(5, NightH);
  EEPROM.get(6, NightM);


  Serial.begin(9600);
}


void loop() {

    if(Serial.available())
    {
      char m = Serial.read();
      if ( m =='1'){mode == "1";}
      if ( m =='2'){mode == "2";}
      if ( m =='3'){mode == "3";}
       
    }
    else {
      mode = 1;
    }

    switch (mode){
      
      case 1 : 
        mode = 1; // run day
        // add condition to day / night                 

        float CO2, avgpH;
        int TDSppm;
        unsigned long currentMillis = millis();

        if (currentMillis - lastSecondMillis >= 1000){
        //each one sec - because of stop flow
          lastSecondMillis = currentMillis;
          int ATOr = analogRead(A1);
          if(ATOr >= 760 && mode == 1){
            if(ATO == false && (millis() - lastDebounceTime) > debounceDelay)
            {
            ATO = true;
            digitalWrite(3, HIGH);
            }
          } else if (ATOr < 760){ //TODO need to meassure it 
            ATO = false;
            digitalWrite(3, LOW);
          }
        };

        if (currentMillis - previousTempMillis >= 300000) {  // 5 minutes interval
            previousTempMillis = currentMillis;
          //main loop meassuring each 5 min  
          tempSen.requestTemperatures();
          float tempC = tempSen.getTempCByIndex(0);  
          TDSppm = analogRead(A2)*(5.0/1023.0)/0.0023;
          
          //meassure current pH each 3 sec to get 100 values in 5 min   
          float pHV = analogRead(A5)*(5.0/1023.0);
          float m = (6.86 - 4.01) / (pH7V - pH4V);
          float b = 4.01 - m * pH4V;
          float pH = m * pHV + b;
          pHBuffer[bufferIndex] = pH;
          bufferIndex = (bufferIndex + 1) % bufferSize;

          //each 5 min 
          avgpH = calculateAvgPH();
          CO2 = 3*KH*pow(10,(7-pH));

          //each 5 min - beacause of buffering and debouncing
          if (CO2 > 15 ) {
            if(mode == 1){
              digitalWrite(3, LOW);
              }
          } else if (CO2 <= 30) {
            digitalWrite(3, HIGH);
          }
        };

      if (currentMillis - previousScaleMillis >= 1800000) {  // 30 minutes interval
            previousScaleMillis = currentMillis;
        //each 30 min
        if (scale.is_ready()){
          int barrelCapacity = scale.read(); //long
        }
        //int barrelCapacity = reading;
          
        //loop 30min
        DateTime dateTime = DS1307.now();
        String header = "Time, CO2, pH, temp, TDS, ATO, relay, barrelCapacity, mode";
        String dataString = String(dateTime.hour()) + ":" + String(dateTime.minute()) + ":" + String(dateTime.second()) +
                          "," + String(CO2) + "," + String(avgpH) + "," + String("temp") +
                          "," + String(TDSppm) + "," + String(ATO) + "," + String("relay") +
                          "," + String("barrelCapacity") + "," + String(mode);  
        File dataLog = SD.open("dataLog.txt", FILE_WRITE);
        if (dataLog) {
          if (dataLog.size() == 0) {
            dataLog.println(header); // print header on the beginnig
          };
        dataLog.println(dataString);
        dataLog.close();
        }
      Serial.println(dataString);
      }
      break;

      case 2:
        mode = 2; // maintenance
        digitalWrite(3, HIGH); //turn relay OFF
        digitalWrite(2, LOW);  //turn ATO OFF
      break;

      case 3:  
        mode = 3; // setup
        if(Serial.available())
        char s = ' ';
        {
          char s = Serial.read();
          if ( s =='1'){
            //need timing
            float pH4V = analogRead(A5)*(5.0/1023.0);
            Serial.print(pH4V);
            //code for pH voltage read
            EEPROM.put(0,pH4V);
            }
          if ( s =='2'){"2";}
          if ( s =='3'){"3";}
        }
      break;
  }
}


float calculateAvgPH() {
  float sum = 0;
  for (int i = 0; i < bufferSize; ++i) {
    sum += pHBuffer[i];
  }
  return sum / bufferSize;
}
