#ifndef AFR_H
#define AFR_H

#include <HardwareSerial.h>

class AFR {
public:
    AFR();
    void begin();
    float readAFR();

private:
    HardwareSerial serial;
};

#endif
