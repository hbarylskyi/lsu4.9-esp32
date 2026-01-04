#ifndef RPM_H
#define RPM_H

#include <Arduino.h>
#include <Wire.h>
#include <INA226.h>

class RPM {
private:
    INA226 ina226;
    uint8_t i2cAddress;
    
    // Frequency calculation variables
    float rpmValue;
    unsigned long lastUpdateTime;
    float lastVoltage;
    float voltageThreshold;
    bool lastState;
    unsigned long lastRiseTime;
    unsigned long lastFallTime;
    float frequency;
    int pulseCount;
    unsigned long lastFreqCalc;
    bool debugEnabled;
    
    // Constants
    static const unsigned long UPDATE_INTERVAL = 10; // Update every 10ms for better frequency detection
    static constexpr float DEFAULT_VOLTAGE_THRESHOLD = 1.65; // 1.65V threshold for AC signal
    static const int PULSES_PER_REVOLUTION = 1; // Adjust based on your trembleur setup
    static constexpr float MIN_FREQUENCY = 0.1; // Minimum frequency to avoid noise
    static const unsigned long FREQ_CALC_WINDOW = 1000; // 1 second window for frequency calculation
    
    // Methods
    void calculateRPM();
    void calculateFrequency();
    
public:
    RPM(uint8_t address = 0x40);
    
    void begin();
    void update();
    int getRPM();
    float getVoltage();
    float getCurrent();
    float getFrequency();
    void setVoltageThreshold(float threshold);
    void setDebugEnabled(bool enabled);
    void printDebugInfo();
};

#endif
