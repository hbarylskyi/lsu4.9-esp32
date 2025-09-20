#include "afr.h"

AFR::AFR() : serial(1), latestAFR(0.0), latestTemperature(0) {} 

void AFR::begin()
{
    serial.begin(115200, SERIAL_8N1, 5, -1); // RX, TX pins
}

bool AFR::readUARTData()
{
    uint8_t buffer[1024];
    static size_t bufferIndex = 0;
    uint8_t afrByte = 0;
    uint8_t tempByte = 0;
    size_t readCount = 0;

    int available = serial.available();
    Serial.print(" Available bytes: ");
    Serial.print(available);

    if (available < 16)
    {
        return false; // Not enough data
    }

    readCount = serial.readBytes(buffer, available);
    // Scan backwards for the latest complete packet
    // Packet format: 0x01, byte1, byte2, 0x02, 0x03, 0x04
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
            return true; // found the freshest packet
        }
    }

    return false;
}                                                                                                                      
                                                                                                                       
float AFR::getLatestAFR() const {                                                                                      
    return latestAFR;                                                                                                  
}                                                                                                                      
                                                                                                                       
int AFR::getLatestTemperature() const {                                                                                
    return latestTemperature;                                                                                          
}      
