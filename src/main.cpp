#include <Arduino.h>
#include "display.h"
#include "uart.h"
#include <Wire.h>

#define SDA_PIN 8   // your chosen SDA pin
#define SCL_PIN 10  // your chosen SCL pin

Display display;
UART uart;

void setup() {
  Serial.begin(115200);
  delay(300); // Wait for Serial to initialize
  
  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("Initializing display...");
  display.begin();
  Serial.println("Display initialized.");
  delay(1000); // prevent watchdog reset
  uart.begin();
}

void loop() {
  Serial.println("Rendering static message on display...");
  delay(1000); // prevent watchdog reset
  display.showData(14.7, 3000); // Example AFR and RPM values
  Serial.println("Message rendered.");
}