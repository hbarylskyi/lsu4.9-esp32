#include "afr.h"

AFR::AFR() : serial(1) {}

void AFR::begin() {
    serial.begin(115200, SERIAL_8N1, 20, 21); // RX, TX pins

bool AFR::readUARTData(uint8_t* buffer, size_t length) {
    while (serial.available() > 0) {
        if (serial.read() == 0x01) {
            if (serial.available() >= length - 1) {
                buffer[0] = 0x01;
                serial.readBytes(buffer + 1, length - 1);
                return true;
            }
        }
    }
    return false;
}

float AFR::parseAFR(const uint8_t* buffer) {
    return buffer[1] / 10.0;
}

float AFR::parseTemperature(const uint8_t* buffer) {
    return buffer[2] + 740;
}
}
