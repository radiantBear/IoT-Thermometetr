#ifndef TEMP_DATA_H
#define TEMP_DATA_H


#include "internet.h"        // Info and functions for communicating with web server
#include "lcd.h"             // Functions for displaying data on LCD screen
#include "hardware.h"        // Info on hardware connections for switch I/O

#define DATA_LENGTH 144    // Define length of data storage (for 10 min avgs.)

// Class to control access to data storage
// Max 65535 observations in interval (10 min avg)
class TempData {
    public:
        TempData();

        // Update everything w/ new reading -- combines max/min check, adding for 10-min avg, adding to total average
        void processData(int8_t data);

        // Print everything from 10-min avg list, starting with oldest value
        void print();

        // Calculate the average for the current interval, add it to list of averages, & reset interval
        void avgInterval();

        // Send the maximum & minimum recorded temperatures to the server if it has different values stored
        void sendMaxMin();

        // Send the 10-minute average array and lifetime average to the server
        void sendToServer();
        
    private:
        // Use for storing 10-min avgs
        int16_t dataArr[DATA_LENGTH]; // Using int8_t for only 1 byte of number: save space
        uint8_t location;  // ^

        // Use for calculating 10-min avgs
        long sumForAvg;
        unsigned short obsForAvg;

        // Use for storing lifetime values
        int8_t minimum, maximum;
        float average;
        unsigned long observations; // Use to accurately update average

        // Use for checking what's been sent to server
        int8_t serverMin, serverMax;

        // Add newNum to the interval that's being averaged
        void addToInterval(int8_t newNum);

        // Update max/min if value is largest/smallest seen so far
        void checkMaxMin(int8_t value);

        // Update the lifetime average of the collected data
        void updateLTAvg(int8_t newNum);

        // Create a string out of the 10-minute average array
        String dataArrToString();

        // Get the data stored on the server & update all properties to match
        void getData();
};


#endif