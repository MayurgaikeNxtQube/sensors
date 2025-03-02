#ifndef RAIN_SENSOR_H
#define RAIN_SENSOR_H

#include <stdint.h>

// UART frame structure
typedef struct {
  uint8_t frameHeader; // Fixed 0x3A
  uint8_t frameFlag;   // Read/Write property + Data Number
  uint16_t frameData;  // Data value
  uint8_t crc;         // CRC-8 checksum
} UARTFrame;

// rainfall status enumeration
typedef enum {
  NO_RAIN = 0,
  LIGHT_RAIN = 1,
  MODERATE_RAIN = 2,
  HEAVY_RAIN = 3
} RainfallStatus;

// system status enumeration
typedef enum {
  SYSTEM_NORMAL = 0,
  INTERNAL_COMM_ERROR = 1,
  LEDA_DAMAGED = 2,
  LEDB_DAMAGED = 3,
  OPTICAL_CALIBRATION_BAD = 4,
  PARAM_WRITE_FAILURE = 5,
  SERIAL_CHECK_ERROR = 6,
  LOW_VOLTAGE_WARNING = 7
} SystemStatus;

void initializeRainSensor();
RainfallStatus getRainfallStatus();
SystemStatus getSystemStatus();
void performOpticalCalibration();
void setRainfallOutputFrequency(uint8_t frequency);
void setRainfallSensitivity(uint16_t V1, uint16_t V2, uint16_t V3, uint16_t S1,
                            uint16_t S2, uint16_t S3, uint8_t N1, uint8_t N2,
                            uint8_t N3);
void enableAmbientLightDetection(bool enable);
uint16_t getAmbientLightValue();
float getModuleTemperature();
void enterSleepMode(bool enable);
void enterRealTimeRainfallMode(bool enable);
uint16_t getRawRainfallValue();
uint16_t getFirmwareVersion();
uint8_t calculateCRC8(const uint8_t *data, uint8_t length);

#endif
