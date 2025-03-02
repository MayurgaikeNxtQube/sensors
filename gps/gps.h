#ifndef GPS_H
#define GPS_H

#include <stdio.h>
#include <string.h>
#include "driver/uart.h"

// UART config
#define GPS_UART_PORT UART_NUM_1
#define GPS_TX_PIN 17
#define GPS_RX_PIN 16
#define GPS_BAUD_RATE 9600
#define GPS_BUFFER_SIZE 1024

void gps_init();
void gps_update();
bool gps_data_available();
float gps_get_latitude();
float gps_get_longitude();
float gps_get_altitude();
float gps_get_speed();
int gps_get_satellites();
char* gps_get_time();
char* gps_get_date();

#endif
