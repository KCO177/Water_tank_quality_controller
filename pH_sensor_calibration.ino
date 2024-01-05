void setup() {

  // Inicializace sériového portu pro výstup a Bluetooth
  Serial.begin(9600);

  // Nastavení pinů pro řízení relé
  pinMode(8, OUTPUT);  // put your setup code here, to run once:

}

void loop() {
    // Read analog voltage from pH sensor
    float voltage = analogRead(A0) * (5.0 / 1024.0);

    // pH calibration points
    //pH4
    float pH4_voltage = 4.199;
    float pH7_voltage = 3.936; 

    // Calculate slope (m) and y-intercept (b) for the linear equation: pH = m * voltage + b
    float m = (6.86 - 4.01) / (pH7_voltage - pH4_voltage);
    float b = 4.01 - m * pH4_voltage;

    // Calculate pH using the linear equation
    float pH = m * voltage + b;

    // Print pH value to the serial monitor
    Serial.print("pH: ");
    Serial.println(pH, 2);
    
    // Uncomment the following line if you want to send pH over Bluetooth
    // BTSerial.println("pH: " + String(pH, 2));

    // Add a delay to control the frequency of pH readings
    delay(1000);
}
