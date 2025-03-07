#ifndef WIND_SENSOR_H
#define WIND_SENSOR_H

#include <Arduino.h>

#define WIND_SENSOR_PIN 34

void wind_sensor_init();
float wind_sensor_read_voltage();
float wind_sensor_get_speed();

#endif // !WIND_SENSOR_H
