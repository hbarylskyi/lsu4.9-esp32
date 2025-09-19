#include <Arduino.h>
#include "display.h"
#include "uart.h"

Display display;
UART uart;

void setup() {
  display.begin();
  uart.begin();
}

void loop() {
  float afr = uart.readAFR();
  int rpm = 0; // Placeholder for RPM value
  display.showData(afr, rpm);
  delay(1000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
