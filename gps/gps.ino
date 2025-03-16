#include <SoftwareSerial.h>

const int RXPin = 4; // TX NEO-7
const int TXPin = 3; // RX NEO-7

SoftwareSerial gpsSerial(RXPin, TXPin);

// Buffer for storing NMEA sentences
const int sentenceSize = 83; // $GPGGA sentences are up to 82 chars
char sentence[sentenceSize];
int charsRead = 0;

// GPS data
float latitude = 0.0;
float longitude = 0.0;
int hour = 0, minute = 0, second = 0;
int day = 0, month = 0, year = 0;
int satellites = 0;
float altitude = 0.0;
bool dataValid = false;

void setup() {
  Serial.begin(9600);
  Serial.println("u-blox NEO-7 GPS");

  gpsSerial.begin(9600);
}

void loop() {
  if (gpsSerial.available()) {
    readGPS();
  }

  static unsigned long startTime = millis();
  if (millis() - startTime > 5000 && charsRead == 0) {
    Serial.println("No GPS data received. Check wiring and antenna.");
    delay(5000);
  }
}

void readGPS() {
  while (gpsSerial.available()) {
    char c = gpsSerial.read();

    // Check if we're at the beginning of a new NMEA sentence
    if (c == '$') {
      charsRead = 0; // Reset the character counter
    }

    // Add character to the sentence buffer
    if (charsRead < sentenceSize - 1) {
      sentence[charsRead++] = c;
      sentence[charsRead] = '\0';
    }

    // If we reached the end of a sentence, process it
    if (c == '\n' && charsRead > 0) {
      if (strstr(sentence, "$GPGGA") != NULL) {
        parseGGA();
        displayGPSInfo();
      } else if (strstr(sentence, "$GPRMC") != NULL) {
        parseRMC();
      }
    }
  }
}

void parseGGA() {
  // Sample GGA sentence:
  // $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
  // 1    = UTC time (123519 = 12:35:19)
  // 2,3  = Latitude (4807.038,N = 48 deg 07.038' N)
  // 4,5  = Longitude (01131.000,E = 11 deg 31.000' E)
  // 6    = Fix quality (0=no fix, 1=GPS fix, 2=DGPS fix)
  // 7    = Satellites tracked
  // 8    = HDOP
  // 9,10 = Altitude (545.4,M = 545.4 meters)
  
  char *token = strtok(sentence, ",");
  int tokenCount = 0;
  
  while (token != NULL) {
    switch (tokenCount) {
      case 0: // GPGGA header
        break;
      case 1: // Time
        if (strlen(token) >= 6) {
          hour = (token[0] - '0') * 10 + (token[1] - '0');
          minute = (token[2] - '0') * 10 + (token[3] - '0');
          second = (token[4] - '0') * 10 + (token[5] - '0');
        }
        break;
      case 2: // Latitude
        if (strlen(token) > 0) {
          float lat = atof(token);
          int degrees = (int)(lat / 100);
          float minutes = lat - (degrees * 100);
          latitude = degrees + (minutes / 60.0);
        }
        break;
      case 3: // N/S indicator
        if (token[0] == 'S') {
          latitude = -latitude;
        }
        break;
      case 4: // Longitude
        if (strlen(token) > 0) {
          float lon = atof(token);
          int degrees = (int)(lon / 100);
          float minutes = lon - (degrees * 100);
          longitude = degrees + (minutes / 60.0);
        }
        break;
      case 5: // E/W indicator
        if (token[0] == 'W') {
          longitude = -longitude;
        }
        break;
      case 6: // Fix quality
        dataValid = (token[0] == '1' || token[0] == '2');
        break;
      case 7: // Number of satellites
        satellites = atoi(token);
        break;
      case 9: // Altitude
        altitude = atof(token);
        break;
    }
    token = strtok(NULL, ",");
    tokenCount++;
  }
}

void parseRMC() {
  // $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
  // Sample RMC sentence:
  // 1    = UTC time
  // 2    = Status (A=active or V=void)
  // 3,4  = Latitude
  // 5,6  = Longitude
  // 7    = Speed in knots
  // 8    = Track angle in degrees
  // 9    = Date (230394 = 23rd of March 1994)
  
  char *token = strtok(sentence, ",");
  int tokenCount = 0;
  
  while (token != NULL) {
    switch (tokenCount) {
      case 9: // Date
        if (strlen(token) >= 6) {
          day = (token[0] - '0') * 10 + (token[1] - '0');
          month = (token[2] - '0') * 10 + (token[3] - '0');
          year = 2000 + (token[4] - '0') * 10 + (token[5] - '0'); // Assuming 21st century
        }
        break;
    }
    token = strtok(NULL, ",");
    tokenCount++;
  }
}

void displayGPSInfo() {
  Serial.println("----------------------------------------");
  
  // Display location info
  if (dataValid) {
    Serial.print("Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
  } else {
    Serial.println("Location: Not Available");
  }

  // Display altitude info
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" meters");

  // Display date/time info
  Serial.print("Date/Time: ");
  Serial.print(month);
  Serial.print("/");
  Serial.print(day);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  
  // Format time with leading zeros
  if (hour < 10) Serial.print("0");
  Serial.print(hour);
  Serial.print(":");
  if (minute < 10) Serial.print("0");
  Serial.print(minute);
  Serial.print(":");
  if (second < 10) Serial.print("0");
  Serial.print(second);
  Serial.println(" UTC");

  // Display satellite count
  Serial.print("Satellites: ");
  Serial.println(satellites);

  Serial.println("----------------------------------------");
  delay(1000); // Update every second
}
