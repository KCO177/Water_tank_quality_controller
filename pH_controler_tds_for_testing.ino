/*
mode 1 - day CO2 on
mode 2 - night CO2 off
mode 3 - maintenance - CO2 off ATO off
*/
//###version for testing wirirng
//#include <Wire.h>
#include<math.h>
const int bufferSize = 300;
float pHBuffer[bufferSize];
int bufferIndex = 0;
int mode = 1;
unsigned long lastDebounceTime = 0;


void setup() {
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH); 
}

void loop() {
    //get voltage from pH
    float pHV = analogRead(A5) * (5.0/1023.0); 
    //get voltage from TDS max 2.3V and recalculate to ppm 
    int TDSppm = analogRead(A2)*(5.0/1023.0)/0.0023;
    //constant from meassurring - need to recalculate dznamical from TDS ppm - analysis
    int KH = 6;
    //pH value from buffer pH4.01 
    float pH4V = 4.238;
    //pH vlaue from buffer pH6.86
    float pH7V = 3.936;
    //current pH calculation
    float m = (6.86 - 4.01) / (pH7V - pH4V);
    float b = 4.01 - m * pH4V;
    float pH = m * pHV + b;
    //buffering current pH to enable bouncing state
    pHBuffer[bufferIndex] = pH;
    bufferIndex = (bufferIndex + 1) % bufferSize;
    float avgpH = calculateAvgPH();
    //CO2 mg/l calculation Henderson-Hasselbalch equation 
    float CO2 = 3*KH*pow(10,(7-pH));

    //Auto Top Off (ATO) 
    int ATOr = analogRead(A1);
    if(ATOr >= 760){
      if (mode != 2 && millis() - lastDebounceTime > 5000){
      mode = 2;
      digitalWrite(2, HIGH);
      }
    }
    //check if the ssr relay has inverse logic high/low on/off 
    if (CO2 > 30) {
      if(mode = 1 && millis() - lastDebounceTime > 5000){
        digitalWrite(3, LOW);
        }
    } else if(CO2 <= 30) {
      if(mode =1 && millis() - lastDebounceTime > 5000){
      digitalWrite(3, HIGH);
    }
  }
}

float calculateAvgPH() {
  float sum = 0;
  for (int i = 0; i < bufferSize; ++i) {
    sum += pHBuffer[i];
  }
  return sum / bufferSize;
}
