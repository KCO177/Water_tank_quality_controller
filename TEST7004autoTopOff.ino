const int analogPin = A0; // specify the analog pin you are using

void setup() {
  // Set digital pins 2 and 3 as outputs
  pinMode(2, OUTPUT); //floater001
  pinMode(3, OUTPUT); //floater002
  pinMode(4, OUTPUT); //floaterBarrel
  Serial.begin(9600);
}

void readAndPrintVoltage() {
  // Read analog voltage
  int sensorValue = analogRead(analogPin); // read the analog voltage
  float voltage = sensorValue * (5.0 / 1023.0); // convert the sensor value to voltage

  Serial.print("Analog Value: ");
  Serial.print(sensorValue);
  Serial.print(", Voltage: ");
  Serial.println(voltage);
}

void loop() {
  // All low
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  Serial.print("L,L,L");
  readAndPrintVoltage();  
  delay(1000);

  // All high
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH); // 1
  digitalWrite(4, HIGH); // 1 = 0
  Serial.print("H,H,H");
  readAndPrintVoltage();
  delay(1000);

  // All low
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  Serial.print("H,H,L");
  readAndPrintVoltage();
  delay(1000);

  // All high
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW); // 1
  digitalWrite(4, HIGH); // 1 = 0
  Serial.print("H,L,H");
  readAndPrintVoltage();
  delay(1000);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW); // 1
  digitalWrite(4, HIGH); // 1 = 0
  Serial.print("L,L,H");
  readAndPrintVoltage();
  delay(1000);
}

