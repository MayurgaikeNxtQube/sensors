#include "gps.h"
#include <cstdlib>

// GPS Data Variables
static char gps_raw_data[GPS_BUFFER_SIZE];
static float latitude = 0.0;
static float longitude = 0.0;
static float altitude = 0.0;
static float speed = 0.0;
static int satellites = 0;
static char time[10] = "";
static char date[10] = "";

// Initialize UART for GPS
void gps_init() {
  uart_config_t uart_config = {.baud_rate = GPS_BAUD_RATE,
                               .data_bits = UART_DATA_8_BITS,
                               .parity = UART_PARITY_DISABLE,
                               .stop_bits = UART_STOP_BITS_1,
                               .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
  uart_param_config(GPS_UART_PORT, &uart_config);
  uart_set_pin(GPS_UART_PORT, GPS_TX_PIN, GPS_RX_PIN, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE);
  uart_driver_install(GPS_UART_PORT, GPS_BUFFER_SIZE * 2, 0, 0, NULL, 0);
}

// updat GPS data
void gps_update() {
  int length = 0;
  uart_get_buffered_data_len(GPS_UART_PORT, (size_t *)&length);
  if (length > 0) {
    length = uart_read_bytes(GPS_UART_PORT, (uint8_t *)gps_raw_data, length,
                             100 / portTICK_PERIOD_MS);
    gps_raw_data[length] = '\0'; // Null-terminate the string
    parse_nmea(gps_raw_data);
  }
}

// check if GPS data is available
bool gps_data_available() { return strlen(gps_raw_data) > 0; }

// get latitude
float gps_get_latitude() { return latitude; }

// get longitude
float gps_get_longitude() { return longitude; }

// get altitude
float gps_get_altitude() { return altitude; }

// get speed
float gps_get_speed() { return speed; }

// get the number of satelites
int gps_get_satellites() { return satellites; }

// get time
char *gps_get_time() { return time; }

// get date
char *gps_get_date() { return date; }

// parce nmea sentence
void parse_nmea(char *data) {
  char *sentence = strtok(data, "\n");
  while (sentence != NULL) {
    if (strstr(sentence, "$GPGGA")) {
      // eg:
      // $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
      char *parts[15];
      int index = 0;
      char *token = strtok(sentence, ",");
      while (token != NULL) {
        parts[index++] = token;
        token = strtok(NULL, ",");
      }
      if (index >= 10) {
        strncpy(time, parts[1], 9);  // time (HHMMSS)
        latitude = atof(parts[2]);   // latitude
        longitude = atof(parts[4]);  // longitude
        satellites = atoi(parts[7]); // number of satellites
        altitude = atof(parts[9]);   // altitude
      }
    } else if (strstr(sentence, "$GPRMC")) {
      // eg:
      // $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
      char *parts[12];
      int index = 0;
      char *token = strtok(sentence, ",");
      while (token != NULL) {
        parts[index++] = token;
        token = strtok(NULL, ",");
      }
      if (index >= 10) {
        strncpy(time, parts[1], 9); // time (HHMMSS)
        strncpy(date, parts[9], 9); // date (DDMMYY)
        speed = atof(parts[7]);     // speed in knots
      }
    }
    sentence = strtok(NULL, "\n");
  }
}
