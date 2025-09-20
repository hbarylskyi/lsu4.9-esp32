#include "display.h"

Display::Display() : u8g2(U8G2_R0) {}

void Display::begin() {
    u8g2.begin();
    u8g2.setI2CAddress(0x3C*2);

    Serial.println("U8g2 initialized successfully");
}

void Display::showData(float afr, int rpm) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, "AFR: ");
    u8g2.setCursor(30, 10);
    u8g2.print(afr);
    u8g2.drawStr(0, 20, "RPM: ");
    u8g2.setCursor(30, 20);
    u8g2.print(rpm);
    u8g2.sendBuffer();
}
