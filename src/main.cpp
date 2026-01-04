#include <Arduino.h>
#include "display.h"
#include "afr.h"
#include "rpm.h"
#include "i2c_helpers.h"
#include <Wire.h>

#define SDA_PIN 8
#define SCL_PIN 10

Display display_i2c;
AFR afr_uart;
RPM rpm_i2c;

void setup()
{
  Serial.begin(115200);
  delay(300); // Wait for Serial to initialize
  Serial.println("starting.");

  Wire.begin(SDA_PIN, SCL_PIN);

  // Check I2C device connections
  checkI2CDevice(0x40, "INA226 RPM Sensor");
  checkI2CDevice(0x3C, "SSD1306 Display"); // SSD1306 typically uses 0x3C
  // Add other I2C device checks here as needed

  display_i2c.begin();
  afr_uart.begin();
  rpm_i2c.begin();
  
  // Enable debug logging for RPM sensor
  rpm_i2c.setDebugEnabled(true);

  Serial.println("all sensors initialized.");

  // scanI2CDevices(); // Call the I2C scan function if needed
  display_i2c.showData(0.0, 0, 0);
}

void loop()
{
  afr_uart.update();
  rpm_i2c.update();

  float afr = afr_uart.getLatestAFR();
  int temperature = afr_uart.getLatestTemperature();
  int rpm = rpm_i2c.getRPM();

  display_i2c.showData(afr, rpm, temperature);

  // Print main sensor data
  Serial.print("AFR: ");
  Serial.print(afr);
  Serial.print(" RPM: ");
  Serial.print(rpm);
  Serial.print(" Freq: ");
  Serial.print(rpm_i2c.getFrequency(), 2);
  Serial.print("Hz Temp: ");
  Serial.print(temperature);
  Serial.print("°C Voltage: ");
  Serial.print(rpm_i2c.getVoltage(), 3);
  Serial.print("V Current: ");
  Serial.print(rpm_i2c.getCurrent(), 3);
  Serial.println("A");

  // Uncomment for detailed RPM debugging
  // rpm_i2c.printDebugInfo();

  delay(10); // Minimal delay for main loop
}
