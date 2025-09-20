#ifndef AFR_H
#define AFR_H

#include <HardwareSerial.h>

class AFR {
public:
    AFR();
    void begin();
    bool readUARTData();
    float getLatestAFR() const;                                                                                        
    int getLatestTemperature() const;  
    
    
private:
    HardwareSerial serial;                                                                                             
    float latestAFR;                                                                                                   
    int latestTemperature;  
};

#endif
