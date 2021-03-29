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
#ifndef _CONFIG_H
#define _CONFIG_H

// Includes
#include "helper.h"
#include <Arduino.h>
#include <ArduinoJson.h>

// defintions
#define CFG_JSON_BUFSIZE    1000

#define CFG_APPNAME         "PressureMon"       // Name of firmware
#define CFG_FILENAME        "/pressuremon.json" // Name of config file

#define WIFI_DEFAULT_SSID   "PressureMon"   // Name of created SSID
#define WIFI_DEFAULT_PWD    "password"      // Password for created SSID
#define WIFI_MDNS           "pressuremon"   // Prefix for MDNS name
#define WIFI_PORTAL_TIMEOUT 120             // Number of seconds until the config portal is closed

// These are used in API + Savefile
#define CFG_PARAM_ID                 "id"
#define CFG_PARAM_MDNS               "mdns"
#define CFG_PARAM_OTA                "ota-url"
#define CFG_PARAM_PUSH_FERMENTRACK   "fermentrack-push"
#define CFG_PARAM_PUSH_BREWFATHER    "brewfather-push"
#define CFG_PARAM_PUSH_HTTP          "http-push"
#define CFG_PARAM_PUSH_INTERVAL      "push-interval"
#define CFG_PARAM_TEMPFORMAT         "temp-format"
#define CFG_PARAM_PRESSURECORR       "pressure-corr"
#define CFG_PARAM_VOLTAGEFACTOR      "voltage-factor"
#define CFG_PARAM_MIN_PRESSURE       "min-pressure"
#define CFG_PARAM_MAX_PRESSURE       "max-pressure"

// These are used in API's
#define CFG_PARAM_APP_NAME           "app-name"
#define CFG_PARAM_APP_VER            "app-ver"
#define CFG_PARAM_TEMP_C             "temp-c"
#define CFG_PARAM_TEMP_F             "temp-f"
#define CFG_PARAM_BATTERY            "battery"
#define CFG_PARAM_SLEEP_MODE         "sleep-mode"
#define CFG_PARAM_RSSI               "rssi"
#define CFG_PARAM_PRESSURE           "pressure"
#define CFG_PARAM_PRESSURE_FORMAT    "pressure-format"

class Config {
    private:
        bool   saveNeeded;

        String id;
        String mDNS;
        String otaURL;
        String fermentrackPushTarget;
        String brewfatherPushTarget;
        String httpPushTarget;
        char   tempFormat;
        float  pressureZeroCorrection;
        int    pushInterval;
        float  voltageFactor;
        int    minPressure;
        int    maxPressure;

        void debug();
        void formatFileSystem();

    public:
        Config();
        const char*  getID() { return id.c_str(); };

        const char*  getMDNS() { return mDNS.c_str(); }
        void         setMDNS( String s ) { mDNS = s; saveNeeded = true; }

        const char*  getOtaURL() { return otaURL.c_str(); }
        void         setOtaURL( String s ) { otaURL = s; saveNeeded = true; }
        bool         isOtaActive() { return otaURL.length()>0?true:false; }

        const char*  getFermentrackPushTarget() { return fermentrackPushTarget.c_str(); }
        void         setFermentrackPushTarget( String s ) { fermentrackPushTarget = s; saveNeeded = true; }
        bool         isFermemtrackActive() { return fermentrackPushTarget.length()>0?true:false; }

        const char*  getBrewfatherPushTarget() { return brewfatherPushTarget.c_str(); }
        void         setBrewfatherPushTarget( String s ) { brewfatherPushTarget = s; saveNeeded = true; }
        bool         isBrewfatherActive() { return brewfatherPushTarget.length()>0?true:false; }

        const char*  getHttpPushTarget() { return httpPushTarget.c_str(); }
        void         setHttpPushTarget( String s ) { httpPushTarget = s; saveNeeded = true; }
        bool         isHttpActive() { return httpPushTarget.length()>0?true:false; }

        int          getPushInterval() { return pushInterval; }
        void         setPushInterval( int v ) { pushInterval = v; saveNeeded = true; }
        void         setPushInterval( String s ) { pushInterval = s.toInt(); saveNeeded = true; }

        int          getMinPressure() { return minPressure; }
        void         setMinPressure( int v ) { minPressure = v; saveNeeded = true; }
        void         setMinPressure( String s ) { minPressure = s.toInt(); saveNeeded = true; }

        int          getMaxPressure() { return maxPressure; }
        void         setMaxPressure( int v ) { maxPressure = v; saveNeeded = true; }
        void         setMaxPressure( String s ) { maxPressure = s.toInt(); saveNeeded = true; }

        char         getTempFormat() { return tempFormat; }
        void         setTempFormat( const char c ) { tempFormat = c; saveNeeded = true; }
        bool         isTempC() { return tempFormat=='C'?true:false; };
        bool         isTempF() { return tempFormat=='F'?true:false; };

        float        getPressureZeroCorrection() { return pressureZeroCorrection; }
        void         setPressureZeroCorrection( float f ) { pressureZeroCorrection = f; saveNeeded = true; }
        void         setPressureZeroCorrection( String s ) { pressureZeroCorrection = s.toFloat(); saveNeeded = true; }

        float        getVoltageFactor() { return voltageFactor; }
        void         setVoltageFactor( float f ) { voltageFactor = f; saveNeeded = true; }
        void         setVoltageFactor( String s ) { voltageFactor = s.toFloat(); saveNeeded = true; }

        // IO functions
        void createJson(DynamicJsonDocument& doc);
        bool saveFile();
        bool loadFile();
        void checkFileSystem();
        bool isSaveNeeded() { return saveNeeded; };
        void setSaveNeeded() { saveNeeded = true; };
};

// Global instance created
extern Config myConfig;

#endif // _CONFIG_H

// EOF