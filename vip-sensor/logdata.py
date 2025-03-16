import serial
import csv
import time
from datetime import datetime

# Serial port configuration
SERIAL_PORT = '/dev/ttyACM0'  # Change as needed
BAUD_RATE = 9600

# CSV file configuration
filename_input = input("Enter filename for CSV (without extension): ")
CSV_FILENAME = f"./data/{filename_input}_{
    datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"


def parse_line(line):
    """Parse a line from the serial output and extract the values."""
    try:
        parts = line.strip().split()
        if len(parts) < 3:
            return None, None, None

        if "raw voltage:" in line:
            raw_value = int(parts[2])
            calculated_value = float(parts[5])
            return "voltage", raw_value, calculated_value
        elif "raw current:" in line:
            raw_value = int(parts[2])
            calculated_value = float(parts[5])
            return "current", raw_value, calculated_value
        return None, None, None
    except (IndexError, ValueError) as e:
        print(f"Error parsing line: {line} - {e}")
        return None, None, None


def main():
    try:
        print(f"Attempting to connect to Arduino on {SERIAL_PORT}...")
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print("Connection established! Waiting for data...")

        time.sleep(2)  # Give Arduino time to reset

        with open(CSV_FILENAME, 'w', newline='') as csv_file:
            csv_writer = csv.writer(csv_file)
            csv_writer.writerow(['Timestamp', 'Raw Voltage', 'Raw Current',
                                'Calculated Voltage (V)', 'Calculated Current (A)', 'Power (W)'])

            print(f"Logging data to {CSV_FILENAME}. Press Ctrl+C to stop.")
            ser.flushInput()

            raw_voltage = None
            calculated_voltage = None
            raw_current = None
            calculated_current = None

            while True:
                voltage_line = ser.readline().decode('utf-8', errors='replace').strip()
                if not voltage_line:
                    continue
                voltage_type, raw_voltage, calculated_voltage = parse_line(
                    voltage_line)
                if voltage_type != "voltage":
                    continue

                current_line = ser.readline().decode('utf-8', errors='replace').strip()
                if not current_line:
                    continue
                current_type, raw_current, calculated_current = parse_line(
                    current_line)
                if current_type != "current":
                    continue

                ser.readline()  # Read empty line after current

                timestamp = datetime.now().strftime(
                    '%Y-%m-%d %H:%M:%S.%f')[:-3]
                power = calculated_voltage * calculated_current

                csv_writer.writerow(
                    [timestamp, raw_voltage, raw_current, calculated_voltage, calculated_current, power])

                print(f"[{timestamp}] Raw Voltage: {raw_voltage}, Raw Current: {raw_current}, Voltage: {
                      calculated_voltage:.2f}V, Current: {calculated_current:.3f}A, Power: {power:.2f}W")

                csv_file.flush()

    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
        print("Please check:")
        print(f"1. The Arduino is connected to {SERIAL_PORT}")
        print("2. The correct port is specified")
        print("3. No other program is using the serial port")
    except KeyboardInterrupt:
        print("\nLogging stopped by user")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial connection closed")


if __name__ == "__main__":
    main()
