#define CURRENT_SENSOR_PIN A0  // DCI -> Current Sensor Output
#define VOLTAGE_SENSOR_PIN A1  // DCV -> Voltage Sensor Output

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Read Current Sensor (ACS712)
    int currentRaw = analogRead(CURRENT_SENSOR_PIN);
    float voltageCurrent = (currentRaw / 1023.0) * 5.0; // Convert ADC value to voltage
    float current = (voltageCurrent - 2.5) / 0.185; // ACS712-5A formula (adjust for your sensor)

    // Read Voltage Sensor (Voltage Divider)
    int voltageRaw = analogRead(VOLTAGE_SENSOR_PIN);
    float measuredVoltage = (voltageRaw / 1023.0) * 5.0; // Convert ADC to voltage
    float actualVoltage = measuredVoltage * 11; // Assuming 11:1 voltage divider

    // Print readings
    Serial.print("raw voltage: "); Serial.print(voltageRaw);
    Serial.print(" calculated voltage: "); Serial.println(actualVoltage);
    
    Serial.print("raw current: "); Serial.print(currentRaw);
    Serial.print(" calculated current: "); Serial.println(current);

    delay(1000);
}
