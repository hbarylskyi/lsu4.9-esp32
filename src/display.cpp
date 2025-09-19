#include "display.h"

Display::Display() : u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ U8X8_PIN_NONE, /* data=*/ U8X8_PIN_NONE, /* cs=*/ U8X8_PIN_NONE, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE) {}

void Display::begin() {
    u8g2.begin();
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
