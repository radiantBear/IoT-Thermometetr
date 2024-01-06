#ifndef LCD_H
#define LCD_H


#include "hardware.h"        // Info on hardware connections for LCD I/O
#include <LiquidCrystal.h>   // Library for LCD screen

#define SEC *1000            // Make it easier to see how many seconds millis() is showing

extern LiquidCrystal lcd;

// Update LCD to show current temperature & how long the program has been running
void updateLCD(int8_t temperature);


#endif