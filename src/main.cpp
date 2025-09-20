#include <Arduino.h>
#include "display.h"
#include "afr.h"
#include "scan_i2c.h"
#include <Wire.h>

#define SDA_PIN 8   // your chosen SDA pin
#define SCL_PIN 10  // your chosen SCL pin

Display display;
AFR afr_uart;

void setup() {
  Serial.begin(115200);
  delay(300); // Wait for Serial to initialize
  Serial.println("starting.");
  
  Wire.begin(SDA_PIN, SCL_PIN);
  display.begin();
  afr_uart.begin();

  Serial.println("all sensors initialized.");
  
  // scanI2CDevices(); // Call the I2C scan function if needed
}

void loop() {
  uint8_t buffer[9];
  if (afr_uart.readUARTData(buffer, sizeof(buffer))) {
    float afr = afr_uart.parseAFR(buffer);
    float temperature = afr_uart.parseTemperature(buffer);
    int rpm = 1000; // Placeholder for RPM value, replace with actual reading if available
    display.showData(afr, rpm);
    Serial.print("AFR: ");
    Serial.print(afr);
    Serial.print(" Temperature: ");
    Serial.println(temperature);
  }
  
  delay(1000); // Update every second
}
