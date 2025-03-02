# Wind Speed Sensor

## How it works?

- Anemometer has 3-4 cups that rotate when the wind blows
- The speed of rotation is proportional to the wind speed
- Sensor converts rotation to corresponding analog voltage output (0-5V)

### Wiring

| Sensor Pin  | Board Pin      |
| ----------- | -------------- |
| VCC         | 5V             |
| GND         | GND            |
| OUT         | GPIO 34(ESP32) |

### Wind speed to voltage conversion table

| Wind speed (m/s) | Output voltage (V) |
| ---------------- | ------------------ |
| 1                | 0.17               |
| 2                | 0.33               |
| 5                | 0.83               |
| 10               | 1.67               |
| 20               | 3.33               |
| 30               | 5.00               |

### Formula for conversion

`Wind speed = (Voltage - 0.17) * 30 / (5.0 - 0.17)`
where 0.17V is the minimum output for 1m/s and 5V is the maximum for 30m/s
