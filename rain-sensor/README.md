# Intelligent Rain Detection Module

### Features:
- Usage conditions:
inside windshields, without direct exposure to external natural environment
- Dimensions:
Diameter: 12.5+/- 0.15mm
- Error status:
actively sends error status to host via UART
- Optical, Temp
  - Self optical system calibration
  - On-board temperature detection, excellent temperature linearity to ensure accurate measurements
  - Supports optical sleep mode to extend svc life of optic components

## Functionalities

### Basic Operations

#### 1. Read Rainfall Status
- No rain
- Light rain
- Moderate rain
- Heavy rain

#### 2. Perform System Diagnostics
- Check internal communication
- Detect LEDA/LEDB failure
- Optical system calibration failure
- Parameter configuration failure
- Serial communication error
- Low voltage warning

#### 3. Optical System Calibration
- Perform calibration
- Retrieve calibration values

#### 4. Real-time Rainfall Mode
- Enable/disable real-time rainfall mode
- Retrieve raw rainfall values

### Advanced Features

#### 5. Set Rainfall Sensitivity Parameters
- Configure thresholds for rain intensity (V1, V2, V3)
- Configure static raindrop distribution (S1, S2, S3)
- Configure rainfall event count thresholds(N1, N2, N3)


#### 6. Set Output Frequency
- Configure frequency of rainfall status reports

#### 7. Ambient Light Detection
- Enable/disable light detection
- Read ambient light value


#### 8. Temperature Detection
- Read temperature sensor value

#### 9. Power Management
- Enter/exit sleep mode

#### 10. Firmware Version Query
- Retrieve firmware version
