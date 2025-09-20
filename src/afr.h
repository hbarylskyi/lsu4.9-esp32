#ifndef AFR_H
#define AFR_H

#include <HardwareSerial.h>

class AFR {
public:
    AFR();
    void begin();
    bool readUARTData(uint8_t* buffer, size_t length);
    float parseAFR(const uint8_t* buffer);
    float parseTemperature(const uint8_t* buffer);

private:
    HardwareSerial serial;
};

#endif
