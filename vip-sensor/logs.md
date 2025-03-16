# Voltage Current Sensor Logs

## Systems
### 1. AC-DC Power System
Expected:
  DCV -> 0-24V
  DCI -> 0-20A

### 2. Motor Drivers

#### M1, M2
Expected:
  DCV -> 24V
  DCI -> 5.6A
  
#### M3, M4, M5, M6
Expected
  DCV -> 24V
  DCI -> 3.2A

### 3. Main unit
#### RPI4
Expected:
  DCV -> 5V
  DCI -> 3A

#### ESP32
Expected:
  DCV -> 5V
  DCI -> 1A

#### PICO
Expected:
  DCV -> 5V
  DCI -> 1A

### 4. DJI Charger
#### Drone | 60W
Expected:
  DCV -> 17.0 or 17.6V
  DCI -> 3.53 or 3.41A

#### RC | 10W
Expected:
  DCV -> 5V
  DCI -> 2A

#### IO Pin 
Expected:
  Voltage Sensor -> OFF==0V ON==5V

### 5. 12V SMPS System
#### Network Switch
Expected:
  DCV -> 5V
  DCI -> 0.6A

#### Odroid
Expected:
  DCV -> 12V
  DCI -> 2A

#### Platform lights
Expected:
  DCV -> 9-12V
  DCI -> assumed max

### 6. Slave 2 System
#### 12V Internal PSU 
Expected:
  DCV -> 12V
  DCI -> max +300mA - 1A

#### ESP32
Expected:
  DCV -> 5V
  DCI -> 800mA - 1A

#### Slave 2 3V3 Bus
Expected:
  DCV -> 3V3
  DCI -> 1A

