#include <Wire.h>
#include <Arduino.h>

#ifndef I2C_HELPERS_H
#define I2C_HELPERS_H

void scanI2CDevices();
bool checkI2CDevice(uint8_t address, const char *deviceName);

#endif // I2C_HELPERS_H
