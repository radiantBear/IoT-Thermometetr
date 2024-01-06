#include "../lcd.h"
#include "../wifi.h"

LiquidCrystal lcd(RS_PIN, EN_PIN, DB0_PIN, DB1_PIN, DB2_PIN, DB3_PIN, DB4_PIN, DB5_PIN, DB6_PIN, DB7_PIN); // Initialize LCD screen

void updateLCD(int8_t temperature) {
    lcd.clear();

    // Tell user if WiFi is connected
    if(wiFiConnected) {
        lcd.print("WiFi connected");
    } else {
        lcd.print("WiFi disconnected");
    }

    // Print out the current temperature in ºF
    lcd.setCursor(0, 1);
    lcd.printf("%d%cF", (int) temperature, 223); // 223 = char code for 'º' on this LCD screen

    // Print out how long the device has been running
    unsigned long  time = millis() / (1 SEC);
    unsigned short seconds = time % 60;
    unsigned short minutes = (time / 60) % 60;
    unsigned short hours = ((time / 60) / 60) % 24;
    unsigned long  days = ((time / 60) / 60) / 24;

    // Print time right-aligned -- millis() overflows approx. every 54 days, so never need >2 digits for days
    lcd.setCursor(5, 1);
    lcd.printf("%2lu:%02hu:%02hu:%02hu", days, hours, minutes, seconds);
}