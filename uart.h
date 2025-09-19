#ifndef UART_H
#define UART_H

#include <HardwareSerial.h>

class UART {
public:
    UART();
    void begin();
    float readAFR();

private:
    HardwareSerial serial;
};

#endif
