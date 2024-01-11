//#include <Wire.h>
#include<math.h>
const int bufferSize = 300;
float pHBuffer[bufferSize];

int bufferIndex = 0;
int mode = 1;

void setup() {
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH); 
}

void loop() {

    float pHV = analogRead(A5) * (5.0 / 1024.0);
    int TDSppm = analogRead(2)/0.0023;
    int KH = 6;
    float pH4V = 4.238;  
    float pH7V = 3.936;
    float m = (6.86 - 4.01) / (pH7V - pH4V);
    float b = 4.01 - m * pH4V;
    float pH = m * pHV + b;
    pHBuffer[bufferIndex] = pH;
    
    bufferIndex = (bufferIndex + 1) % bufferSize;
    float avgpH = calculateAvgPH();
    float CO2 = 3*KH*pow(10,(7-pH));

    int ATOr = analogRead(A1);
    if(ATOr >= 760){
      mode = 2;
      digitalWrite(2, HIGH);
    }
     
    if (CO2 > 15 ) {
      if(mode = 1){
        digitalWrite(3, LOW);
        }
    } else if (CO2 <= 30) {
      digitalWrite(3, HIGH);
    }
}



float calculateAvgPH() {
  float sum = 0;
  for (int i = 0; i < bufferSize; ++i) {
    sum += pHBuffer[i];
  }
  return sum / bufferSize;
}
