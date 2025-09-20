#include "afr.h"

AFR::AFR() : serial(1) {}

void AFR::begin() {
    serial.begin(115200, SERIAL_8N1, 20, 21); // RX, TX pins
}

bool AFR::readUARTData() {
    static std::vector<uint8_t> localBuffer;
    while (serial.available() > 0) {
        localBuffer.push_back(serial.read());
    }

    auto it = std::find(localBuffer.begin(), localBuffer.end(), 0x01);
    while (it != localBuffer.end() && std::distance(it, localBuffer.end()) >= 8) {
        if (*(it + 3) == 0x02) {
            buffer[0] = 0x01;
            std::copy(it + 1, it + 8, buffer + 1);
            localBuffer.erase(localBuffer.begin(), it + 8);
            return true;
        }
        it = std::find(it + 1, localBuffer.end(), 0x01);
    }

    if (it != localBuffer.end()) {
        localBuffer.erase(localBuffer.begin(), it);
    } else {
        localBuffer.clear();
    }

    return false;
}

float AFR::parseAFR(const uint8_t* buffer) {
    return buffer[1] / 10.0;
}

float AFR::parseTemperature(const uint8_t* buffer) {
    return buffer[2] + 740;
}
