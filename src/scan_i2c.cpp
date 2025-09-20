#include <Arduino.h>
#include <Wire.h>

#define SDA_PIN 8   // your chosen SDA pin
#define SCL_PIN 10  // your chosen SCL pin

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nI2C Scanner starting...");

  // Initialize I2C on your chosen pins
  Wire.begin(SDA_PIN, SCL_PIN);
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("Done.\n");
  }

  delay(3000); // wait before next scan
}