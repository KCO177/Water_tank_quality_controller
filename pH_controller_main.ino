#include <SPI.h>
#include <SD.h>
#include <math.h>
#include "RTClib.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include "HX711.h"

RTC_DS1307 DS1307;

const float pH4V = 4.238;  
const float pH7V = 3.936;
const int bufferSize = 100;
float pHBuffer[bufferSize];
int bufferIndex = 0;
int mode = 1;
const int sd_CS = 10;
bool ATO = true;
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;

HX711 scale;

OneWire oneWireDs(7);
DallasTemperature tempSen(&oneWireDs);

void setup() {
  pinMode(2, OUTPUT); // Assuming pin 2 is used for ATO
  pinMode(3, OUTPUT); // Assuming pin 3 is used for relay
  digitalWrite(3, HIGH);
  DS1307.adjust(DateTime(F(__DATE__), F(__TIME__)));
  tempSen.begin();
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.begin(9600);
}


void loop() {
  
    tempSen.requestTemperatures();
    float tempC = tempSen.getTempCByIndex(0);  
    int TDSppm = analogRead(A2)*(5.0/1023.0)/0.0023;
    int KH = 6;
       
    float pHV = analogRead(A5)*(5.0/1023.0);
    float m = (6.86 - 4.01) / (pH7V - pH4V);
    float b = 4.01 - m * pH4V;
    float pH = m * pHV + b;
    pHBuffer[bufferIndex] = pH;
    bufferIndex = (bufferIndex + 1) % bufferSize;
    float avgpH = calculateAvgPH();
    float CO2 = 3*KH*pow(10,(7-pH));

    int ATOr = analogRead(A1);
    if(ATOr >= 760 && mode == 1){
      ATO = true;
      digitalWrite(2, HIGH);
    } else if (ATOr < 760){
      ATO = false;
      digitalWrite(2, LOW);
    }
    
    if (CO2 > 15 ) {
      if(mode == 1){
        digitalWrite(3, LOW);
        }
    } else if (CO2 <= 30) {
      digitalWrite(3, HIGH);
    }

    if (scale.is_ready()){
      int barrelCapacity = scale.read(); //int
    }
    //int barrelCapacity = reading;
    
    //loop 30min
    DateTime dateTime = DS1307.now();
    String header = "Time, CO2, pH, temp, TDS, ATO, relay, barrelCapacity, mode";
    String dataString = String(dateTime.hour()) + ":" + String(dateTime.minute()) + ":" + String(dateTime.second()) +
                      "," + String(CO2) + "," + String(avgpH) + "," + String("temp") +
                      "," + String(TDSppm) + "," + String("ATO") + "," + String("relay") +
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

float calculateAvgPH() {
  float sum = 0;
  for (int i = 0; i < bufferSize; ++i) {
    sum += pHBuffer[i];
  }
  return sum / bufferSize;
}
