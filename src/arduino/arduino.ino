/* Data Logger
 *
 * Collect and store data from a DS18B20 probe thermometer
 * Range is -127ºF - 128ºF, accuracy ± 1ºF
 *
 * Nathaniel Baird
 * bairdn@oregonstate.edu
 *
 * 05/18/2023 - 06/02/2023
 */

/* CITATIONS
 *
 * LCD panel tutorial: https://learn.sparkfun.com/tutorials/basic-character-lcd-hookup-guide
 *
 * POST and GET request tutorial: https://randomnerdtutorials.com/esp32-http-get-post-arduino/
 *
 * WPA2 Enterprise "magic": JeroenBeemster (https://github.com/JeroenBeemster/ESP32-WPA2-enterprise)
 * WiFi Scan: expressif ESP32 documentation (WiFi scan example)
 */

#include "hardware.h"        // Info on hardware connections for switch I/O
#include "wifi.h"        // Info and functions for connecting to WiFi
#include "lcd.h"         // Functions for displaying data on LCD screen
#include "thermometer.h" // Functions for collecting data from the thermometer
#include "temp_data.h"   // Class for controlling data storage

// General utility info
#define SEC         *1000  // Make it easier to see how many seconds millis() is showing


// Arduino IDE function for code to run on startup
void setup() {
    // Set up serial port
    Serial.begin(115200);

    // Prep status for switch
    pinMode(SWITCH, OUTPUT);

    // Prep screen
    lcd.begin(16, 2);

    // Prep thermometer
    if(!ds.selectNext() && digitalRead(SWITCH) == HIGH)
        Serial.println("Thermometer not found!");
    // Print diagnostic info if desired
    if(digitalRead(SWITCH) == HIGH)
        Serial.printf("Thermometers detected: %d\n", ds.getNumberOfDevices());

    // Show connection info on LCD
    lcd.clear();
    lcd.print("Connecting to ");
    lcd.setCursor(0, 1);
    lcd.printf("%s...", WIFI_SSID);
    
    // Attempt to connect to WiFi
    connectToWiFi();

    // Print diagnostic info if desired
    if(digitalRead(SWITCH) == HIGH)
        Serial.println("Beginning normal operation.");
}


// Arduino IDE function for code to run indefinitely
void loop() {
    static unsigned long lastRead = 0, lastAvg = 0, lastUpload = 0; // Only initializes first time
    static TempData timeArr;

    // Each second, print the sensor's reading & save reading for 10 minute average calculation
    // Subtract to protect against millis() overflow
    if(millis() - lastRead >= 1 SEC) {
        // Update timing for most recent print -- FIRST so each reading is 1 sec apart, not 1 sec + time to execute this
        lastRead = millis();

        // Get reading from sensor
        int8_t reading = getTemp();

        // process reading if valid
        if(reading != INT8_MIN) {

            // Use reading for data (only need 1 data pt/sec -- NOT 100,000 data pts/sec (600,000,000 data pts/10 min))
            timeArr.processData(reading);

            // Output current temperature
            updateLCD(reading);

            // Print current reading over Serial if switch shows granular data is desired
            if(digitalRead(SWITCH) == HIGH) 
                Serial.printf("Current temperature: %dºF\n", reading);
        } else {
            // Print error message if switch shows granular data is desired
            if(digitalRead(SWITCH) == HIGH)
                Serial.println("ERROR: Failed to read temperature");
        }

        // Update server w/ info every 30 seconds
        // Nested to allow access to 'int8_t reading'
        if(millis() - lastUpload >= 30 SEC) {
            lastUpload = millis();
            postData("current=" + String(reading));
            timeArr.sendMaxMin();
        }
    }

    // Every 10 minutes, calculate and store the average temperature from the last 10 minutes
    // Subtract to protect against millis() overflow
    if(millis() - lastAvg >= 600 SEC) {    
        // Update timing for most recent average -- FIRST so each average is 10 min apart, not 10 min + time to execute this
        lastAvg = millis();

        // Avg this interval & add to avg list
        timeArr.avgInterval();

        timeArr.sendToServer();
    }

    // Check if input from USB; print summary if command was sent
    if(Serial.available()) {
        String input = Serial.readStringUntil('\n');
        if(input.equalsIgnoreCase("print")) {
            timeArr.print();
        }
    }
}