#ifndef AFR_H
#define AFR_H

#include <HardwareSerial.h>

class AFR {
public:
    AFR();
    void begin();
    bool readUARTData();
    float parseAFR(const uint8_t* buffer);
    float parseTemperature(const uint8_t* buffer);

private:
    HardwareSerial serial;
};

#endif
