#ifndef WIFI_H
#define WIFI_H


#include "hardware.h"        // Info on hardware connections for switch I/O
#include "secrets.h"         // Login credentials
#include "esp_wpa2.h"        // Library for connecting to WPA2-Enterprise Wi-Fi (ie OSU's)
#include <WiFi.h>            // Library for internet connection

#define WIFI_SSID "eduroam"


// Set global var to print WiFi connection status on LCD
extern bool wiFiConnected;


// Attempt to connect to WiFi using credentials in secrets.h
// Returns true if successful or false if it timed out
bool connectToWiFi();


#endif