#include "../temp_data.h"

TempData::TempData() {
    // Initialize using -128º for undefined (outside sensor range anyway)
    location = DATA_LENGTH - 1;
    for(int i = 0; i < DATA_LENGTH; i++) {
        dataArr[i] = INT8_MIN;
    }

    sumForAvg = 0;
    obsForAvg = 0;

    minimum = INT8_MAX;
    maximum = INT8_MIN;
    serverMin = INT8_MAX;
    serverMax = INT8_MIN;

    average = 0;

    // Tell user about next step
    lcd.clear();
    lcd.print("Checking for");
    lcd.setCursor(0, 1);
    lcd.print("uploaded data...");

    // Check the server for saved data; restore that data if it's there
    getData();
}


void TempData::processData(int8_t data) {
    addToInterval(data); // Add data to 10 min avg 
    checkMaxMin(data);   // Update max/min to include new reading
    updateLTAvg(data);   // Update lifetime average with new reading
}


void TempData::print() {
    // Print max, min, avg
    Serial.printf("\nLifetime maximum temperature: %dºF\n", maximum);
    Serial.printf("Lifetime minimum temperature: %dºF\n", minimum);
    Serial.printf("Lifetime average temperature: %.1fºF\n", average);

    // Print 10-min avgs
    Serial.println("10-minute averages from past 24 hours:");
    Serial.println(dataArrToString());

    Serial.println();
}


void TempData::avgInterval() {
    // Calculate average
    int16_t avg = round((float) sumForAvg / obsForAvg * 10); // round from float for accuracy

    // Reset vars for next interval calculation
    sumForAvg = 0;
    obsForAvg = 0;

    // Store data
    // Move to next spot
    ++location %= DATA_LENGTH; // Same as location = (location + 1) % DATA_LENGTH -- prevents overflow without if statement
    // Overwrite whatever was there (now >24hrs old)
    dataArr[location] = avg;
    // Update long-term storage as well
}


void TempData::sendMaxMin() {
    if(minimum != serverMin || maximum != serverMax) {
        postData("minimum=" + String(minimum) + "&maximum=" + String(maximum));
        serverMin = minimum;
        serverMax = maximum;
    }
}


void TempData::sendToServer() {
    postData("averages=" + dataArrToString() + "&fullAverage=" + String(average, 1) + "&observations=" + String(obsForAvg));
}


void TempData::addToInterval(int8_t newNum) {
    sumForAvg += newNum;
    obsForAvg++;
}


void TempData::checkMaxMin(int8_t value) {
    if(value > maximum) maximum = value;
    if(value < minimum) minimum = value;
}


void TempData::updateLTAvg(int8_t newNum) {
    observations++;
    if(observations == 0) observations = ULONG_MAX; // If it overflowed somehow, just keep it @ highest value -- it'll still be pretty accurate

    // Calculate average: (old average * old # of observations) = weighted for adding new number & re-averageing
    average = (average * (observations - 1) + newNum) / observations;
}


String TempData::dataArrToString() {
    // Find which array node is oldest (one after most recent overwrite) for 10-min avgs
    long first = (location + 1 < DATA_LENGTH) ? location + 1 : 0;
    String output;
    
    // Loop thru array to print, starting w/ oldest value & looping around if past end of array: incrementer is same as location = (location + 1) % DATA_LENGTH -- prevents overflow without if statement
    for(int i = first; i != location; ++i %= DATA_LENGTH) {
        // Print corresponding reading if it's been set
        if(dataArr[i] != INT8_MIN)
            // output += String(dataArr[i]) + ',';
            // snprintf(nextLine, "%.1f, ", (float) dataArr[i] / 10), 5);
            output += String(dataArr[i] / 10) + '.' + String(dataArr[i] % 10) + ',';
    }

    // Add most recent reading if it's been set w/ new line (had to break out of for() before printing this one to prevent infinite loop)
    if(dataArr[location] != INT8_MIN) {
        output += String(dataArr[location] / 10) + '.' + String(dataArr[location] % 10);
        // output += String(dataArr[location]);
    }
    
    return output;
}


void TempData::getData() {
    if(digitalRead(SWITCH) == HIGH)
        Serial.println("Checking server for saved data...");
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
        // Create object for WiFi API
        WiFiClientSecure client;

        // Allow secure connection using root certificate authority certificate
        client.setCACert(ROOT_CA_CERT);

        // Create object for HTTP API
        HTTPClient http;
    
        // Open connection to server over WiFi
        http.begin(client, SERVER);
        
        // Send HTTP GET request
        int httpResponseCode = http.GET();

        // Record server response
        String responseData = http.getString();

        // Parse the data if it was given
        if(responseData != "") {
            JSONVar myObject = JSON.parse(responseData);
            if(JSON.typeof(myObject != "undefined")) {
                // Parse the response to update all properties
                
                this->minimum = int(myObject["minimum"]);
                this->serverMin = minimum;
                this->maximum = int(myObject["maximum"]);
                this->serverMax = maximum;

                this->average = double(myObject["fullAverage"]);
                this->obsForAvg = (unsigned long) myObject["observations"];

                // Restore 10-minute averages
                int i;
                int high = myObject["averages"].length();
                for(i = 0; i < high; i++) {
                    this->dataArr[i % DATA_LENGTH] = double(myObject["averages"][i]) * 10;
                }
                this->location = (i - 1) % DATA_LENGTH;

                // Print diagnostic info if desired
                if(digitalRead(SWITCH) == HIGH) {
                    Serial.print("Restored uploaded data:");
                    this->print();
                }
            } else {
                // Print diagnostic info if desired
                if(digitalRead(SWITCH) == HIGH) 
                    Serial.print("No data found.");
                // Tell server to disregard old data if there was an error
                postData("current=-&averages=-");
            }
        } else {
            // Print diagnostic info if desired
            if(digitalRead(SWITCH) == HIGH) 
                Serial.print("No data found.");
            // Tell server to disregard any old data if it didn't send any saved data
            postData("current=-&averages=-");
        }
            
        // Free resources
        http.end();

        // Alert user if abnormal response code
        if(httpResponseCode != 200 && digitalRead(SWITCH) == HIGH)  {
            Serial.print("Abnormal HTTP Response code: ");
            Serial.println(httpResponseCode);
        }
    }
    else {
        // Alert user if WiFi is disconnected & they want diagnostic info
        if(digitalRead(SWITCH) == HIGH)
            Serial.println("WiFi Disconnected");
    }
}