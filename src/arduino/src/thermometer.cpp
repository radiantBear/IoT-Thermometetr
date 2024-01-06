#include "../thermometer.h"

DS18B20 ds(THERMOMETER); // Initialize thermometer

int8_t getTemp() {
    // Read temperature as Fahrenheit -- takes ~250 ms
    float t = ds.getTempF();

    // Return early if failed to read
    if(isnan(t)) {
        return INT8_MIN; // Use for error since -128º is outside the sensor range anyway
    }
    return (int8_t) round(t);
}