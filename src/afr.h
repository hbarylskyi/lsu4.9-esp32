#ifndef AFR_H
#define AFR_H

#include <HardwareSerial.h>

class AFR {
public:
    AFR();
    void begin();
    void update();
    float getLatestAFR() const;                                                                                        
    int getLatestTemperature() const;  
    
    
private:
    HardwareSerial serial;                                                                                             
    float latestAFR;                                                                                                   
    int latestTemperature;
    unsigned long lastUpdateTime;
    static const unsigned long UPDATE_INTERVAL = 50; // Update every 50ms
};

#endif
