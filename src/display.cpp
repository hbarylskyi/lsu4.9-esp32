#include "display.h"

Display::Display() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void Display::begin() {
    if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
        Serial.println("SSD1306 allocation failed");
        for (;;);
    }
    Serial.println("SSD1306 initialized successfully");
    display.clearDisplay();
    display.display();
}

void Display::showData(float afr, int rpm) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("AFR: ");
    display.println(afr);
    display.print("RPM: ");
    display.println(rpm);
    display.display();
}
