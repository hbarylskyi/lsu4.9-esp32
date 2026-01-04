#include "rpm.h"

RPM::RPM(uint8_t address) : ina226(address), i2cAddress(address) {
    rpmValue = 0.0;
    lastUpdateTime = 0;
    lastVoltage = 0.0;
    voltageThreshold = DEFAULT_VOLTAGE_THRESHOLD;
    lastState = false;
    lastRiseTime = 0;
    lastFallTime = 0;
    frequency = 0.0;
    pulseCount = 0;
    lastFreqCalc = 0;
    debugEnabled = false;
}

void RPM::begin() {
    Serial.println("Initializing INA226 RPM sensor...");
    
    // Initialize INA226 sensor
    if (ina226.begin()) {
        Serial.println("✓ INA226 sensor found at address 0x40");
        
        // Configure INA226 for voltage/current measurement
        Serial.println("Configuring INA226 calibration...");
        int error = ina226.setMaxCurrentShunt(20.0, 0.002000000095F, true);
        if (error == 0) {
            Serial.println("✓ INA226 calibration successful");
            Serial.print("  - Current LSB: ");
            Serial.print(ina226.getCurrentLSB_mA());
            Serial.println(" mA");
            Serial.print("  - Shunt resistance: ");
            Serial.print(ina226.getShunt());
            Serial.println(" Ω");
            Serial.print("  - Max current: ");
            Serial.print(ina226.getMaxCurrent());
            Serial.println(" A");
        } else {
            Serial.print("✗ INA226 calibration error: 0x");
            Serial.println(error, HEX);
        }
        
        // Set continuous mode for both shunt and bus voltage
        ina226.setModeShuntBusContinuous();
        Serial.println("✓ Set to continuous measurement mode");
        
        // Set averaging to reduce noise (4 samples)
        ina226.setAverage(4);
        Serial.println("✓ Set averaging to 4 samples");
        
        // Set conversion time to 1.1ms for good balance of speed and accuracy
        ina226.setBusVoltageConversionTime(4);
        ina226.setShuntVoltageConversionTime(4);
        Serial.println("✓ Set conversion time to 1.1ms");
        
        // Test initial readings
        float testVoltage = ina226.getBusVoltage();
        float testCurrent = ina226.getCurrent();
        Serial.print("✓ Initial readings - Voltage: ");
        Serial.print(testVoltage);
        Serial.print("V, Current: ");
        Serial.print(testCurrent);
        Serial.println("A");
        
    } else {
        Serial.println("✗ INA226 sensor initialization failed!");
        Serial.println("  Check I2C connection and address (0x40)");
    }
    
    lastUpdateTime = millis();
    lastFreqCalc = millis();
    Serial.println("RPM sensor initialization complete");
}

void RPM::update() {
    unsigned long currentTime = millis();
    
    // Only update if enough time has passed
    if (currentTime - lastUpdateTime < UPDATE_INTERVAL) {
        return;
    }
    
    lastUpdateTime = currentTime;
    
    // Read voltage from INA226
    float currentVoltage = ina226.getBusVoltage();
    // float currentCurrent = ina226.getCurrent();
    
    if (debugEnabled) {
        Serial.print("[RPM DEBUG] Voltage: ");
        Serial.print(currentVoltage, 3);
        // Serial.print("V, Current: ");
        // Serial.print(currentCurrent, 3);
        Serial.print("A, Threshold: ");
        Serial.print(voltageThreshold, 3);
        Serial.print("V, State: ");
        Serial.println(currentVoltage > voltageThreshold ? "HIGH" : "LOW");
    }
    
    // Detect state changes for frequency calculation
    bool currentState = currentVoltage > voltageThreshold;
    
    // Detect rising edge
    if (currentState && !lastState) {
        unsigned long riseTime = micros();
        if (lastRiseTime > 0) {
            unsigned long period = riseTime - lastRiseTime;
            if (period > 1000) { // Minimum 1ms period to avoid noise
                float instantFreq = 1000000.0 / period; // Convert microseconds to Hz
                if (debugEnabled) {
                    Serial.print("[RPM DEBUG] Rising edge detected! Period: ");
                    Serial.print(period);
                    Serial.print("μs, Instant freq: ");
                    Serial.print(instantFreq, 2);
                    Serial.println(" Hz");
                }
            }
        }
        lastRiseTime = riseTime;
        pulseCount++;
        
        if (debugEnabled) {
            Serial.print("[RPM DEBUG] Pulse count: ");
            Serial.println(pulseCount);
        }
    }
    
    // Detect falling edge
    if (!currentState && lastState) {
        lastFallTime = micros();
        if (debugEnabled) {
            Serial.println("[RPM DEBUG] Falling edge detected!");
        }
    }
    
    lastState = currentState;
    lastVoltage = currentVoltage;
    
    // Calculate frequency every second
    if (currentTime - lastFreqCalc >= FREQ_CALC_WINDOW) {
        calculateFrequency();
        lastFreqCalc = currentTime;
    }
    
    // Calculate RPM
    calculateRPM();
}

void RPM::calculateFrequency() {
    if (pulseCount > 0) {
        frequency = pulseCount / (FREQ_CALC_WINDOW / 1000.0);
        if (debugEnabled) {
            Serial.print("[RPM DEBUG] Frequency calculation - Pulses: ");
            Serial.print(pulseCount);
            Serial.print(", Window: ");
            Serial.print(FREQ_CALC_WINDOW);
            Serial.print("ms, Frequency: ");
            Serial.print(frequency, 2);
            Serial.println(" Hz");
        }
    } else {
        frequency = 0.0;
        if (debugEnabled) {
            Serial.println("[RPM DEBUG] No pulses detected in calculation window");
        }
    }
    pulseCount = 0;
}

void RPM::calculateRPM() {
    if (frequency > MIN_FREQUENCY) {
        // Convert frequency to RPM
        // RPM = (Frequency in Hz * 60) / Pulses per revolution
        rpmValue = (frequency * 60.0) / PULSES_PER_REVOLUTION;
        if (debugEnabled) {
            Serial.print("[RPM DEBUG] RPM calculation - Freq: ");
            Serial.print(frequency, 2);
            Serial.print("Hz, RPM: ");
            Serial.print(rpmValue, 1);
            Serial.println();
        }
    } else {
        rpmValue = 0.0;
    }
}

int RPM::getRPM() {
    return (int)rpmValue;
}

float RPM::getVoltage() {
    return lastVoltage;
}

float RPM::getCurrent() {
    return ina226.getCurrent();
}

float RPM::getFrequency() {
    return frequency;
}

void RPM::setVoltageThreshold(float threshold) {
    voltageThreshold = threshold;
    Serial.print("Voltage threshold set to: ");
    Serial.print(threshold, 3);
    Serial.println("V");
}

void RPM::setDebugEnabled(bool enabled) {
    debugEnabled = enabled;
    Serial.print("RPM debug logging ");
    Serial.println(enabled ? "enabled" : "disabled");
}

void RPM::printDebugInfo() {
    Serial.println("=== RPM SENSOR DEBUG INFO ===");
    Serial.print("Voltage: ");
    Serial.print(lastVoltage, 3);
    Serial.print("V (threshold: ");
    Serial.print(voltageThreshold, 3);
    Serial.println("V)");
    Serial.print("Current: ");
    Serial.print(ina226.getCurrent(), 3);
    Serial.println("A");
    Serial.print("Frequency: ");
    Serial.print(frequency, 2);
    Serial.println(" Hz");
    Serial.print("RPM: ");
    Serial.print(rpmValue, 1);
    Serial.println();
    Serial.print("Pulse count: ");
    Serial.print(pulseCount);
    Serial.println();
    Serial.print("State: ");
    Serial.println(lastState ? "HIGH" : "LOW");
    Serial.println("=============================");
}