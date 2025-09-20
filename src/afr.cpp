#include "afr.h"

AFR::AFR() : serial(1) {}

void AFR::begin() {
    serial.begin(115200, SERIAL_8N1, 20, 21); // RX, TX pins
}

bool AFR::readUARTData() {
    static uint8_t localBuffer[64];
    static size_t bufferIndex = 0;

    while (serial.available() > 0 && bufferIndex < sizeof(localBuffer)) {
        localBuffer[bufferIndex++] = serial.read();
    }

    for (size_t i = 0; i <= bufferIndex - 8; ++i) {
        if (localBuffer[i] == 0x01 && localBuffer[i + 3] == 0x02) {
            std::copy(localBuffer + i, localBuffer + i + 8, buffer);
            bufferIndex -= (i + 8);
            std::memmove(localBuffer, localBuffer + i + 8, bufferIndex);
            return true;
        }
    }

    if (bufferIndex >= 8) {
        bufferIndex = 0; // Reset if no valid data found
    }

    return false;
}

float AFR::parseAFR(const uint8_t* buffer) {
    return buffer[1] / 10.0;
}

float AFR::parseTemperature(const uint8_t* buffer) {
    return buffer[2] + 740;
}
