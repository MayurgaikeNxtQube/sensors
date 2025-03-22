#include <hp_BH1750.h>
hp_BH1750 sensor;

void setup() {
  Serial.begin(9600);
  bool avail = sensor.begin(BH1750_TO_GROUND); // initialize sensor with addr pin connected to ground

  if(!avail) {
    Serial.println("No BH1750 sensor found!");
    while (true);
  }
}

void loop() {
  sensor.start(); // start measurement
  float lux = sensor.getLux();
  Serial.println(lux);
}

