#include "afr.h"

// Read data packets from LSU 4.9 sensor via UART
// Packet format: 0x01, byte1, byte2, 0x02, 0x03, 0x04
// byte1: AFR * 10 (e.g., 135 for AFR 13.5)
// byte2: Temperature - 740 (e.g., 25C -> 25 + 740 = 765)
AFR::AFR() : serial(1), latestAFR(0.0), latestTemperature(0), lastUpdateTime(0) {}

void AFR::begin()
{
    serial.begin(115200, SERIAL_8N1, 5, -1); // RX, TX pins
    lastUpdateTime = millis();
}

void AFR::update()
{
    unsigned long currentTime = millis();
    
    // Only update if enough time has passed
    if (currentTime - lastUpdateTime < UPDATE_INTERVAL) {
        return;
    }
    
    lastUpdateTime = currentTime;
    
    uint8_t buffer[512];
    size_t readCount = 0;

    int available = serial.available();
    
    if (available < 16)
    {
        return; // Not enough data
    }

    readCount = serial.readBytes(buffer, available);
    // Scan backwards for the latest complete packet
    for (int i = readCount - 6; i >= 0; i--)
    {
        if (buffer[i] == 0x01 &&
            buffer[i + 3] == 0x02 &&
            buffer[i + 4] == 0x03 &&
            buffer[i + 5] == 0x04)
        {
            // Extract data bytes
            uint8_t byte1 = buffer[i + 1];
            uint8_t byte2 = buffer[i + 2];

            float afr = byte1 / 10.0;
            int temp = byte2 + 740;

            latestAFR = afr;
            latestTemperature = temp;
            break; // found the freshest packet
        }
    }
}

float AFR::getLatestAFR() const
{
    return latestAFR;
}

int AFR::getLatestTemperature() const
{
    return latestTemperature;
}
