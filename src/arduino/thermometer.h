#ifndef THERMOMETER_H
#define THERMOMETER_H


#include "hardware.h"        // Info on hardware connections for thermometer I/O
#include <DS18B20.h>         // Library for thermometer

extern DS18B20 ds;

// Get the temperature reading from the sensor
int8_t getTemp();


#endif