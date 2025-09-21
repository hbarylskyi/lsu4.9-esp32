#include "display.h"

Display::Display() : u8g2(U8G2_R0) {}

void Display::begin()
{
    u8g2.begin();
    u8g2.setI2CAddress(0x3C * 2);

    Serial.println("U8g2 initialized successfully");
}

void Display::showData(float afr, int rpm, int temperature)
{
       u8g2.clearBuffer();

    // --- Top 16 pixels: yellow background for temperature ---
    u8g2.setDrawColor(1); // 1 = filled (for monochrome, it will be "on")
    u8g2.drawBox(0, 0, u8g2.getDisplayWidth(), 16);

    // Set text color to inverse if your display supports it
    u8g2.setDrawColor(0); // 0 = text will be "off" on the filled box
    u8g2.setFont(u8g2_font_12x6LED_tf);
    u8g2.setCursor(2, 12);  // leave small padding
    u8g2.print("Temp: ");
    u8g2.print(temperature);

    // --- Reset draw color for normal text ---
    u8g2.setDrawColor(1);

    // --- Large AFR in the middle ---
    u8g2.setFont(u8g2_font_profont29_mf);
    int afrX = 30;
    int afrY = 40;
    afr = floor(afr * 10) / 10.0;  // truncate to 1 decimal
    u8g2.setCursor(afrX, afrY);
    u8g2.print(afr);

    // --- RPM below AFR ---
    u8g2.setFont(u8g2_font_12x6LED_tf);
    u8g2.setCursor(10, afrY + 16);
    u8g2.print(rpm);

    u8g2.sendBuffer();
}
