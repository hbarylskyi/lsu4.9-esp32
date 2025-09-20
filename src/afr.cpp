#include "afr.h"

AFR::AFR() : serial(1) {}

void AFR::begin() {
    serial.begin(115200, SERIAL_8N1, 20, 21); // RX, TX pins
}

uint8_t* buffer = new uint8_t[8];

// Read data from UART and validate packet structure
bool AFR::readUARTData() {
    while (serial.available() > 0) {
        uint8_t b = serial.read();

        if (b == 0x01) { // packet start
            size_t got = serial.readBytes(buffer + 1, length - 1);
            if (got == length - 1) {
                buffer[0] = 0x01;

                // Validate static sequence
                if (buffer[3] == 0x02 &&
                    buffer[4] == 0x03 &&
                    buffer[5] == 0x04 &&
                    buffer[6] == 0x05 &&
                    buffer[7] == 0x06) {
                    return true; // valid packet
                }
            }
        }
    }
    return false; // nothing valid
}

float AFR::parseAFR(const uint8_t* buffer) {
    return buffer[1] / 10.0;
}

float AFR::parseTemperature(const uint8_t* buffer) {
    return buffer[2] + 740;
}
