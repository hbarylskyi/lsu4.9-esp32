#include <Arduino.h>
#include "display.h"
#include "uart.h"

Display display;
UART uart;

void setup() {
  Serial.begin(115200);
  delay(300); // Wait for Serial to initialize
  Serial.println("Initializing display...");
  display.begin();
  Serial.println("Display initialized.");
}

void loop() {
  Serial.println("Rendering static message on display...");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("System Initialized");
  display.display();
  delay(5000); // Delay to keep the message on screen
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
