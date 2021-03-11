/*
MIT License

Copyright (c) 2021 Magnus

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include "pushtarget.h"
#include "config.h"

PushTarget myPushTarget;

//
// Send the pressure value
//
void PushTarget::send(float pressure, float temp, bool force ) {
    unsigned long timePassed = abs( millis() - ms );
    unsigned long interval = myConfig.getPushIntervalAsInt()*1000;

    if( ( timePassed < interval ) && !force) {
#if LOG_LEVEL==6
//      Log.verbose(F("PUSH: Timer has not expired %l %l." CR), timePassed, interval );
#endif
        return;
    }

#if LOG_LEVEL==6
//  Log.verbose(F("PUSH: Sending data." CR) );
#endif
    ms = millis();

    if( myConfig.isFermemtrackActive() )
        sendFermentrack( pressure, temp );

    if( myConfig.isBrewfatherActive() )
       sendBrewfather( pressure, temp );

    if( myConfig.isHttpActive() )
       sendHttp( pressure, temp );
}

//
// Send data to fermentrack
//
void PushTarget::sendFermentrack(float pressure, float temp ) {
    // Send data to the fermentrack endpoint
    Log.notice(F("PUSH: Sending values to fermentrack pressure=%F, temp=%F." CR), pressure, temp );

    StaticJsonDocument<400> doc;

    // For testing purposes we use the iSpindel endpoint at fermentrack
    // Fermentrack will only use the angle so we set the C constant in fermentrack to 1 in order to get a graph
    doc["name"]        = myConfig.getMDNS();
    doc["ID"]          = ESP.getChipId();
    doc["token"]       = "fermentrack"; 
    doc["angle"]       = reduceFloatPrecision(  pressure );                      
    doc["temperature"] = reduceFloatPrecision( temp ); 
    doc["temp_units"]  = myConfig.getTempFormat(); 
    doc["battery"]     = reduceFloatPrecision( myBatteryVoltage.getVoltage() ); 
    doc["gravity"]     = 0; 
    doc["interval"]    = myConfig.getPushIntervalAsInt(); 
    doc["RSSI"]        = WiFi.RSSI(); 

    WiFiClient client;
    HTTPClient http;
    String serverPath = myConfig.getFermentrackPushTarget();

    // Your Domain name with URL path or IP address with path
    http.begin( client, serverPath);
    String json;
    serializeJson(doc, json);
#if LOG_LEVEL==6
    Log.verbose(F("PUSH: url %s." CR), serverPath.c_str());
    Log.verbose(F("PUSH: json %s." CR), json.c_str());
#endif
    // Send HTTP GET request
    http.addHeader("Content-Type", "application/json" );
    int httpResponseCode = http.POST(json);
    
    if (httpResponseCode==200) {
        Log.notice(F("PUSH: HTTP Response code %d" CR), httpResponseCode);
    } else {
        Log.error(F("PUSH: HTTP Response code %d" CR), httpResponseCode);
    }

    http.end();
}

//
// Send data to brewfather
//
void PushTarget::sendBrewfather(float pressure, float temp ) {
    // Send data to the brewfather endpoint
    Log.notice(F("PUSH: Sending values to brewfather pressure=%F, temp=%F." CR), pressure, temp );

    StaticJsonDocument<400> doc;
    /*
        {
        "name": "YourDeviceName", // Required field, this will be the ID in Brewfather
        "temp": 20.32,
        "aux_temp": 15.61, // Fridge Temp
        "ext_temp": 6.51, // Room Temp
        "temp_unit": "C", // C, F, K
        "gravity": 1.042,
        "gravity_unit": "G", // G, P
        "pressure": 10,
        "pressure_unit": "PSI", // PSI, BAR, KPA
        "ph": 4.12,
        "bpm": 123, // Bubbles Per Minute
        "comment": "Hello World",
        "beer": "Pale Ale"
        "battery": 4.98
        }
    */
    doc["name"]          = myConfig.getMDNS();
    doc["temp"]          = reduceFloatPrecision( temp );
  //doc["aux_temp"]      = 0;
  //doc["ext_temp"]      = 0;
    doc["temp_unit"]     = myConfig.getTempFormat(); 
    doc["pressure"]      = reduceFloatPrecision( pressure ); 
    doc["pressure_unit"] = "PSI"; 
    doc["battery"]       = reduceFloatPrecision( myBatteryVoltage.getVoltage() ); 
  //doc["gravity"]       = 0;
  //doc["gravity_unit"]  = "G";
  //doc["ph"]            = 0;
  //doc["bpm"]           = 0;
  //doc["comment"]       = "";
  //doc["beer"]          = "";

    WiFiClient client;
    HTTPClient http;
    String serverPath = myConfig.getBrewfatherPushTarget();

    // Your Domain name with URL path or IP address with path
    http.begin( client, serverPath);
    String json;
    serializeJson(doc, json);
#if LOG_LEVEL==6
    Log.verbose(F("PUSH: url %s." CR), serverPath.c_str());
    Log.verbose(F("PUSH: json %s." CR), json.c_str());
#endif

    // Send HTTP POST request
    http.addHeader("Content-Type", "application/json" );
    int httpResponseCode = http.POST(json);

    if (httpResponseCode==200) {
        Log.notice(F("PUSH: HTTP Response code %d" CR), httpResponseCode);
    } else {
        Log.error(F("PUSH: HTTP Response code %d" CR), httpResponseCode);
    }

    http.end();
}

//
// Send data to http target
//
void PushTarget::sendHttp(float pressure, float temp ) {
    // Send data to the standard HTTP endpoint
    Log.notice(F("PUSH: Sending values to http pressure=%F, temp=%F." CR), pressure, temp );

    StaticJsonDocument<400> doc;

    doc["name"]          = myConfig.getMDNS();
    doc["temp"]          = temp;
    doc["temp_unit"]     = myConfig.getTempFormat(); 
    doc["pressure"]      = reduceFloatPrecision( pressure ); 
    doc["pressure_unit"] = "PSI"; 
    doc["battery"]       = reduceFloatPrecision( myBatteryVoltage.getVoltage() ); 
    doc["rssi"]          = WiFi.RSSI(); 

    WiFiClient client;
    HTTPClient http;
    String serverPath = myConfig.getHttpPushTarget();

    // Your Domain name with URL path or IP address with path
    http.begin( client, serverPath);
    String json;
    serializeJson(doc, json);
#if LOG_LEVEL==6
    Log.verbose(F("PUSH: url %s." CR), serverPath.c_str());
    Log.verbose(F("PUSH: json %s." CR), json.c_str());
#endif

    // Send HTTP POST request
    http.addHeader("Content-Type", "application/json" );
    int httpResponseCode = http.POST(json);
    
    if (httpResponseCode==200) {
        Log.notice(F("PUSH: HTTP Response code %d" CR), httpResponseCode);
    } else {
        Log.error(F("PUSH: HTTP Response code %d" CR), httpResponseCode);
    }

    http.end();
}

// EOF 