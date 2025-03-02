#include "rain_sensor.h"
#include <HardwareSerial.h>

// use UART2
#define UART_PORT Serial2

// communication setup
#define UART_BAUD_RATE 115200
#define FRAME_HEADER 0x3A

// initialize the rain sensor
void initializeRainSensor() {
  UART_PORT.begin(UART_BAUD_RATE); // Initialize UART communication
}

// calculate CRC-8 checksum
uint8_t calculateCRC8(const uint8_t *data, uint8_t length) {
  uint8_t crc = 0xFF;
  for (uint8_t i = 0; i < length; i++) {
    crc ^= data[i];
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

// send a command and receive a response
UARTFrame sendUARTCommand(uint8_t frameFlag, uint16_t frameData) {
  UARTFrame frame;
  frame.frameHeader = FRAME_HEADER;
  frame.frameFlag = frameFlag;
  frame.frameData = frameData;
  frame.crc = calculateCRC8((uint8_t *)&frame.frameFlag, 3);

  // send frame
  UART_PORT.write((uint8_t *)&frame, sizeof(frame));

  // wait for response
  UARTFrame response;
  if (UART_PORT.readBytes((uint8_t *)&response, sizeof(response)) ==
      sizeof(response)) {
    if (calculateCRC8((uint8_t *)&response.frameFlag, 3) == response.crc) {
      return response;
    }
  }
  // return invalid frame if CRC fails
  return {0, 0, 0, 0};
}

// get rainfall status
RainfallStatus getRainfallStatus() {
  UARTFrame response = sendUARTCommand(0x01, 0x0000);
  return static_cast<RainfallStatus>(response.frameData & 0x03);
}

// get system status
SystemStatus getSystemStatus() {
  UARTFrame response = sendUARTCommand(0x02, 0x0000);
  return static_cast<SystemStatus>(response.frameData & 0x07);
}

// perform optical system calibration
void performOpticalCalibration() { sendUARTCommand(0x83, 0x0000); }

// set rainfall output frequency
void setRainfallOutputFrequency(uint8_t frequency) {
  sendUARTCommand(0x85, frequency);
}

// set rainfall detection sensitivity
void setRainfallSensitivity(uint16_t V1, uint16_t V2, uint16_t V3, uint16_t S1,
                            uint16_t S2, uint16_t S3, uint8_t N1, uint8_t N2,
                            uint8_t N3) {
  sendUARTCommand(0x86, V1);
  sendUARTCommand(0x87, V2);
  sendUARTCommand(0x88, V3);
  sendUARTCommand(0x89, S1);
  sendUARTCommand(0x8A, S2);
  sendUARTCommand(0x8B, S3);
  sendUARTCommand(0x8C, N1);
  sendUARTCommand(0x8D, N2);
  sendUARTCommand(0x8E, N3);
}

// enable/disable ambient light detection
void enableAmbientLightDetection(bool enable) {
  sendUARTCommand(0x8F, enable ? 0x0001 : 0x0000);
}

// get ambient light value
uint16_t getAmbientLightValue() {
  UARTFrame response = sendUARTCommand(0x8F, 0x0000);
  return response.frameData;
}

// get module temperature
float getModuleTemperature() {
  UARTFrame response = sendUARTCommand(0x10, 0x0000);
  return (response.frameData - 605.36) / -1.5596; // Formula from datasheet
}

// enter/exit sleep mode
void enterSleepMode(bool enable) {
  sendUARTCommand(0x91, enable ? 0x0001 : 0x0000);
}

// enter/exit real-time rainfall mode
void enterRealTimeRainfallMode(bool enable) {
  sendUARTCommand(0x84, enable ? 0x0001 : 0x0000);
}

// get raw rainfall value
uint16_t getRawRainfallValue() {
  UARTFrame response = sendUARTCommand(0x84, 0x0000);
  return response.frameData;
}

// get firmware version
uint16_t getFirmwareVersion() {
  UARTFrame response = sendUARTCommand(0x00, 0x0000);
  return response.frameData;
}
