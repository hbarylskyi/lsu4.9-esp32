#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8g2lib.h>

class Display {
public:
    Display();
    void begin();
    void showData(float afr, int rpm);

private:
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
};

#endif
