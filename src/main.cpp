#include <Arduino.h>
#include "display.h"
#include "uart.h"

Display display;
UART uart;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing display...");
  display.begin();
  Serial.println("Display initialized.");
  uart.begin();
}

void loop() {
  Serial.println("Reading AFR value...");
  float afr = uart.readAFR();
  Serial.print("AFR: ");
  Serial.println(afr);
  int rpm = 0; // Placeholder for RPM value
  Serial.println("Updating display with AFR and RPM values...");
  display.showData(afr, rpm);
  Serial.println("Display updated.");
  delay(1000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
