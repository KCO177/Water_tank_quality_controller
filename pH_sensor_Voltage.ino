const int analogPin = A0;  // Analog pin connected to the pH sensor

void setup() {
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  // Read analog voltage from pH sensor
  int rawValue = analogRead(analogPin);
  float voltage = rawValue * (5.0 / 1024.0);  // Convert to voltage

  // Print the raw value and voltage to the serial monitor
  Serial.print("Raw Value: ");
  Serial.print(rawValue);
  Serial.print(", Voltage: ");
  Serial.println(voltage, 3);  // Print voltage with 3 decimal places

  delay(1000);  // Add a delay to control the frequency of readings
}