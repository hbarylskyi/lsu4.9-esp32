#include "afr.h"

AFR::AFR() : serial(1) {}

void AFR::begin() {
    serial.begin(115200, SERIAL_8N1, 20, 21); // RX, TX pins
}
bool AFR::readUARTData() {
    static uint8_t localBuffer[64];
    static size_t bufferIndex = 0;
    uint8_t afrByte  = 0;
    uint8_t tempByte = 0;

    // Collect new bytes
    while (serial.available() > 0 && bufferIndex < sizeof(localBuffer)) {
        localBuffer[bufferIndex++] = serial.read();
    }

    // Scan for valid packet
    for (int i = 0; i + 7 < bufferIndex; i++) {
        if (localBuffer[i] == 0x01 && localBuffer[i + 3] == 0x02) {
            // Found start of packet
            afrByte  = localBuffer[i + 1]; // byte1
            tempByte = localBuffer[i + 2]; // byte2

            // Remove everything up to end of this packet
            int remaining = bufferIndex - (i + 8);
            // memmove(localBuffer, localBuffer + i + 8, remaining);
            bufferIndex = remaining;

            return true; // Got a packet
        }
    }

    // If buffer is too full without finding packet, reset
    if (bufferIndex > 56) bufferIndex = 0;

    return false;
}

float AFR::parseAFR(const uint8_t* buffer) {
    return buffer[1] / 10.0;
}

float AFR::parseTemperature(const uint8_t* buffer) {
    return buffer[2] + 740;
}
