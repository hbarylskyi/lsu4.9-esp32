#include "display.h"

Display::Display() : u8g2(U8G2_R0) {}

void Display::begin() {
    u8g2.begin();
    u8g2.setI2CAddress(0x3C*2);

    Serial.println("U8g2 initialized successfully");
}

void Display::showData(float afr, int rpm, int temperature) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr); // Use a larger font
    u8g2.drawStr(0, 10, "AFR: ");
    u8g2.setCursor(30, 10);
    u8g2.print(afr);
    u8g2.drawStr(0, 20, "Temp: ");
    u8g2.setCursor(50, 20);
    u8g2.print(temperature);
    u8g2.drawStr(0, 30, "RPM: ");
    u8g2.setCursor(50, 30);
    u8g2.print(rpm);
    u8g2.sendBuffer();
}
