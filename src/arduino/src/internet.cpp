#include "../internet.h"
#include "../wifi.h"

void postData(String data) {
    //Check WiFi connection status
    if(WiFi.status() == WL_CONNECTED){
        wiFiConnected = true;
        // Create object for WiFi API
        WiFiClientSecure client;

        // Allow secure connection using root certificate authority certificate
        client.setCACert(ROOT_CA_CERT);

        // Create object for HTTP API
        HTTPClient http;
    
        // Open connection to server over WiFi
        http.begin(client, SERVER);
        
        // Set header for how data is being sent
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // Set data to send with HTTP POST
        String httpRequestData = data;           
        // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData);
            
        // Free resources
        http.end();

        // Alert the user if abnormal response code
        if(httpResponseCode != 200 && digitalRead(SWITCH) == HIGH) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
        }
    }
    else {
        wiFiConnected = false;
        // Alert the user if WiFi had a problem
        if(digitalRead(SWITCH) == HIGH)
            Serial.println("WiFi Disconnected");
    }
}