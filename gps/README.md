# NEO-7 Global Navigation Satellite System(GNSS) Module

### Features

- GNSS Suppport: GPS, GLONASS, QZSS, SBAS
- Interfaces:
  - UART: 9600 baud default, configurable
  - USB 2.0
  - SPI (up to 1Mb/s): can be enabled by setting D_SEL pin low
  - I2C compatible DDC(up to 400kb/s): for communication with external devices or cellular modules
- Continuous(better performance) and Power Saver(low power) Modes
- Sensitivity: -162 dBm (tracking), -148 dBm (cold start)
- Time-to-first-fix(TTFF): 29s cold start, 1s hot start
- Data logging: available on NEO-7N (16Mb SQI FLASH memory)
- Power Supply: 2.7V to 3.6V(NEO-7N), 1.65V to 3.6V(NEO-7M)

### Working

- Module receives signals from multiple GNSS satellites
- It processes signals to calculate position, velocity, and time (PVT) data
- The data is output via UART, USB, or SPI in NMEA or UBX protocol formats

## Functionalities

### Initialize the GPS Module:
- Set up the UART/USB/SPI interface
- Configure baud rate, data format, and protocols (NMEA/UBX)

### Read Raw GPS Data:
- Read NMEA sentences (e.g., GGA, RMC, GSV) from the module
- Parse the data to extract useful information (latitude, longitude, altitude, speed, etc.)

### Configure GPS Settings:
- Set operating mode (Continuous Mode or Power Save Mode).
- Enable/disable specific GNSS systems (GPS, GLONASS, QZSS)
- Configure update rate (e.g., 1Hz, 5Hz, 10Hz)

### Handle External Interrupts:
- Use the `EXTINT` pin for time aiding or power-saving control

### Data Logging (NEO-7N):
- Store position, velocity, and time data in the internal FLASH memory
- Retrieve logged data for analysis

### Time Pulse Output:
- Configure the `TIMEPULSE` pin to generate a 0PPS (pulse per second) signal

### Error Handling:
- Detect and handle errors like no satellite fix, invalid data, or communication failures
