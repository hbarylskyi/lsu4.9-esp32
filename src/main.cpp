#include <Arduino.h>
#include "display.h"
#include "afr.h"
#include "rpm.h"
#include "scan_i2c.h"
#include <Wire.h>

#define SDA_PIN 8  // your chosen SDA pin
#define SCL_PIN 10 // your chosen SCL pin

Display display;
AFR afr_uart;
RPM rpm_sensor(0x40); // I2C address for trembleur sensor

// Function to check I2C device connection
bool checkI2CDevice(uint8_t address, const char *deviceName)
{
  Wire.beginTransmission(address);
  uint8_t error = Wire.endTransmission();

  if (error == 0)
  {
    Serial.print(deviceName);
    Serial.println(" connected successfully");
    return true;
  }
  else
  {
    Serial.print(deviceName);
    Serial.println(" connection failed");
    return false;
  }
}

void setup()
{
  Serial.begin(115200);
  delay(300); // Wait for Serial to initialize
  Serial.println("starting.");

  Wire.begin(SDA_PIN, SCL_PIN);

  // Check I2C device connections
  checkI2CDevice(0x40, "RPM Sensor");
  // Add other I2C device checks here as needed

  display.begin();
  afr_uart.begin();
  rpm_sensor.begin();

  Serial.println("all sensors initialized.");

  scanI2CDevices(); // Call the I2C scan function if needed
  display.showData(0.0, 0, 0);
}

void loop()
{
  static unsigned long lastReadTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastReadTime >= 50)
  {
    afr_uart.readUARTData();
    rpm_sensor.update(); // Update RPM sensor readings
    lastReadTime = currentTime;
  }
  
  float afr = afr_uart.getLatestAFR();
  int temperature = afr_uart.getLatestTemperature();
  float rpm = rpm_sensor.getRPM(); // Get actual RPM reading from sensor
  display.showData(afr, (int)rpm, temperature);

  Serial.print("AFR: ");
  Serial.print(afr);
  Serial.print(" RPM: ");
  Serial.print(rpm);
  Serial.print(" Temperature: ");
  Serial.println(temperature);

  // Uncomment the line below for detailed RPM debugging
  rpm_sensor.printDebugInfo();

  delay(60); // Main loop delay
}
