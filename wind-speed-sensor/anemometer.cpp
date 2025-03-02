#include "anemometer.h"
#include "esp_adc_cal.h"

#define ADC_MAX_VALUE 4095
#define ADC_REF_VOLTAGE 3.3 // esp32 adc reference voltage

void wind_sensor_init() {
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(WIND_SENSOR_ADC_CHANNEL, ADC_ATTEN_DB_11);
}

float wind_sensor_read_voltage() {
  int raw = adc1_get_raw(WIND_SENSOR_ADC_CHANNEL);
  float voltage = ((float)raw / ADC_MAX_VALUE) * ADC_REF_VOLTAGE;
  return voltage;
}

float wind_sensor_get_speed(float voltage) {
  // mapping voltage (0.17V - 5V) to wind speed (1 m/s - 30 m/s)
  float speed = (voltage - 0.17) * (30.0 / (5.0 - 0.17));
  return (speed < 0) ? 0 : speed;
}
