#include "uart.h"

UART::UART() : serial(1) {}

void UART::begin() {
    serial.begin(9600, SERIAL_8N1, 16, 17); // RX, TX pins
}

float UART::readAFR() {
    if (serial.available()) {
        return serial.parseFloat();
    }
    return 0.0;
}
