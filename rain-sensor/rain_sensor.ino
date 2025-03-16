#include <HardwareSerial.h>

#define UART_PORT Serial2
#define UART_BAUD_RATE 115200
#define FRAME_HEADER 0x3A

struct UARTFrame {
  uint8_t frameHeader; // Fixed 0x3A
  uint8_t frameFlag;   // Read/Write property + Data Number
  uint16_t frameData;  // Data value
  uint8_t crc;         // CRC-8 checksum
};

enum RainfallStatus {
  NO_RAIN = 0,
  LIGHT_RAIN = 1,
  MODERATE_RAIN = 2,
  HEAVY_RAIN = 3
};

enum SystemStatus {
  SYSTEM_NORMAL = 0,
  INTERNAL_COMM_ERROR = 1,
  LEDA_DAMAGED = 2,
  LEDB_DAMAGED = 3,
  OPTICAL_CALIBRATION_BAD = 4,
  PARAM_WRITE_FAILURE = 5,
  SERIAL_CHECK_ERROR = 6,
  LOW_VOLTAGE_WARNING = 7
};

// CRC-8 checksum calculation
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

UARTFrame sendUARTCommand(uint8_t frameFlag, uint16_t frameData) {
  UARTFrame frame;
  frame.frameHeader = FRAME_HEADER;
  frame.frameFlag = frameFlag;
  frame.frameData = frameData;
  frame.crc = calculateCRC8((uint8_t *)&frame.frameFlag, 3);

  // Send frame
  UART_PORT.write((uint8_t *)&frame, sizeof(frame));

  // Wait for response
  UARTFrame response;
  if (UART_PORT.readBytes((uint8_t *)&response, sizeof(response)) == sizeof(response)) {
    if (calculateCRC8((uint8_t *)&response.frameFlag, 3) == response.crc) {
      return response;
    }
  }
  // Return an invalid frame if CRC fails
  return {0, 0, 0, 0};
}

void initializeRainSensor() {
  UART_PORT.begin(UART_BAUD_RATE);
  Serial.println("Rain Sensor Initialized.");
}

RainfallStatus getRainfallStatus() {
  UARTFrame response = sendUARTCommand(0x01, 0x0000);
  return static_cast<RainfallStatus>(response.frameData & 0x03);
}

SystemStatus getSystemStatus() {
  UARTFrame response = sendUARTCommand(0x02, 0x0000);
  return static_cast<SystemStatus>(response.frameData & 0x07);
}

void performOpticalCalibration() {
  sendUARTCommand(0x83, 0x0000);
}

void setRainfallOutputFrequency(uint8_t frequency) {
  sendUARTCommand(0x85, frequency);
}

void enableAmbientLightDetection(bool enable) {
  sendUARTCommand(0x8F, enable ? 0x0001 : 0x0000);
}

uint16_t getAmbientLightValue() {
  UARTFrame response = sendUARTCommand(0x8F, 0x0000);
  return response.frameData;
}

float getModuleTemperature() {
  UARTFrame response = sendUARTCommand(0x10, 0x0000);
  return (response.frameData - 605.36) / -1.5596; // Formula from datasheet
}

void enterSleepMode(bool enable) {
  sendUARTCommand(0x91, enable ? 0x0001 : 0x0000);
}

void enterRealTimeRainfallMode(bool enable) {
  sendUARTCommand(0x84, enable ? 0x0001 : 0x0000);
}

uint16_t getRawRainfallValue() {
  UARTFrame response = sendUARTCommand(0x84, 0x0000);
  return response.frameData;
}

uint16_t getFirmwareVersion() {
  UARTFrame response = sendUARTCommand(0x00, 0x0000);
  return response.frameData;
}

void setup() {
  Serial.begin(115200);
  initializeRainSensor();
}

void loop() {
  // Read sensor values
  RainfallStatus rainStatus = getRainfallStatus();
  SystemStatus sysStatus = getSystemStatus();
  uint16_t ambientLight = getAmbientLightValue();
  float temperature = getModuleTemperature();

  // Print values to Serial Monitor
  Serial.print("Rainfall Status: ");
  switch (rainStatus) {
    case NO_RAIN: Serial.println("No Rain"); break;
    case LIGHT_RAIN: Serial.println("Light Rain"); break;
    case MODERATE_RAIN: Serial.println("Moderate Rain"); break;
    case HEAVY_RAIN: Serial.println("Heavy Rain"); break;
  }

  Serial.print("System Status: ");
  Serial.println(sysStatus);

  Serial.print("Ambient Light: ");
  Serial.println(ambientLight);

  Serial.print("Module Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(2000); // Wait 2 seconds before reading again
}
