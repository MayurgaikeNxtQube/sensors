#ifndef WIND_SENSOR_H
#define WIND_SENSOR_H

#include "driver/adc.h"

#define WIND_SENSOR_ADC_CHANNEL ADC1_CHANNEL_6 // GPIO 34

void wind_sensor_init();
float wind_sensor_read_voltage();
float wind_sensor_get_speed();

#endif // !WIND_SENSOR_H
