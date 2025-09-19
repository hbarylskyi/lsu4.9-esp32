#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SSD1306_I2C_ADDRESS 0x3C

class Display {
public:
    Display();
    void begin();
    void showData(float afr, int rpm);

private:
    Adafruit_SSD1306 display;
};

#endif
