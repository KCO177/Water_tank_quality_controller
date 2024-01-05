#include <SoftwareSerial.h>

SoftwareSerial bluetooth(10, 11); // RX, TX pins pro Bluetooth modul HC-05
int relayPin = 8; // Pin pro ovládání SSR Relay Modulu

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay vypnutý na začátku
  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("Arduino Bluetooth Relay Control");
}

void loop() {
  if (bluetooth.available() > 0) {
    char command = bluetooth.read();

    switch (command) {
      case '1':
        digitalWrite(relayPin, LOW); // Zapni Relay
        Serial.println("Relay zapnut");
        break;
      case '0':
        digitalWrite(relayPin, HIGH); // Vypni Relay
        Serial.println("Relay vypnut");
        break;
      default:
        break;
    }
  }
}