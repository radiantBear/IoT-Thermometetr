#include "../wifi.h"

bool wiFiConnected = false;

bool connectToWiFi() {
    int connectAttempts = 0;
    // WPA2 enterprise magic starts here
    // Credit: JeroenBeemster: https://github.com/JeroenBeemster/ESP32-WPA2-enterprise
    WiFi.disconnect(true);

    // Credit: expressif ESP32 WiFi scan example
    // Scan for networks first -- not part of WPA2 Enterprise magic, but necessary for some reason?
    if(digitalRead(SWITCH) == HIGH)
        Serial.println("Scanning for WiFi networks...");
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();

    // Print diagnostic info if desired
    if(digitalRead(SWITCH) == HIGH) {
        Serial.println("scan done");
        Serial.printf("%d networks found\n", n);
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
        Serial.println("");
    }

    // Back to WPA2 enterprise magic (from JeroenBeemster)
    if(digitalRead(SWITCH) == HIGH)
        Serial.printf("Setting WiFi configuration SSID %s...\n", WIFI_SSID);
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ID, strlen(EAP_ID));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
    esp_wifi_sta_wpa2_ent_enable();
    // WPA2 enterprise magic ends here

    // Connect to network w/ credentials from secrets.h
    WiFi.begin(WIFI_SSID);
    if(digitalRead(SWITCH) == HIGH)
        Serial.print("Connecting to WiFi");
    while(WiFi.status() != WL_CONNECTED) {
        connectAttempts++;
        if(digitalRead(SWITCH) == HIGH)
            Serial.print(".");

        // Time out connection after trying for 10 seconds
        if(connectAttempts > 20) {
            wiFiConnected = false;
            if(digitalRead(SWITCH) == HIGH) 
                Serial.println("ERROR: Could not connect to WiFi. Timed out after 10 seconds.");
            return false;
        }

        delay(500);
    }

    // Update global var for lcd readout
    wiFiConnected = true;

    // Print diagnostic info if desired
    if(digitalRead(SWITCH) == HIGH) {
        Serial.println("");
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("MAC address: ");
        Serial.println(WiFi.macAddress());
    }

    return true;
}