#include <Arduino.h>

#define WIND_SENSOR_PIN A0
#define ADC_MAX_VALUE 1023
#define ADC_REF_VOLTAGE 5.0

void wind_sensor_init() {
  pinMode(WIND_SENSOR_PIN, INPUT);
}

float wind_sensor_read_voltage() {
  int raw = analogRead(WIND_SENSOR_PIN);
  float voltage = ((float)raw / ADC_MAX_VALUE) * ADC_REF_VOLTAGE;
  return voltage;
}

float wind_sensor_get_speed(float voltage) {
  // Mapping voltage (0.17V - 5V) to wind speed (1 m/s - 30 m/s)
  float speed = (voltage - 0.17) * (30.0 / (5.0 - 0.17));
  return (speed < 0) ? 0 : speed;
}

void setup() {
  Serial.begin(115200);
  wind_sensor_init();
  Serial.println("Anemometer Initialized.");
}

void loop() {
  float voltage = wind_sensor_read_voltage();
  float windSpeed = wind_sensor_get_speed(voltage);

  Serial.print("Wind Speed: ");
  Serial.print(windSpeed);
  Serial.println(" m/s");

  delay(2000);
}
