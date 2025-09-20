#include "afr.h"

AFR::AFR() : serial(1) {}

void AFR::begin() {
    serial.begin(115200, SERIAL_8N1, 20, 21); // RX, TX pins
}

float AFR::readAFR() {
    if (serial.available()) {
        return serial.parseFloat();
    }
    return 0.0;
}
