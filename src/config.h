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
#define CFG_APPNAME         "PressureMon"       // Name of firmware
#define CFG_APPVER          "0.2.0"             // Version of firmware (used wit OTA update to check for newer)
#define CFG_FILENAME        "/pressuremon.json" // Name of config file

#define WIFI_DEFAULT_SSID   "PressureMon"   // Name of created SSID
#define WIFI_DEFAULT_PWD    "password"      // Password for created SSID
#define WIFI_MDNS           "pressuremon"   // Prefix for MDNS name
#define WIFI_PORTAL_TIMEOUT 120             // Number of seconds until the config portal is closed

#define CFG_PARAM_ID                 "id"
#define CFG_PARAM_MDNS               "mdns"
#define CFG_PARAM_OTA                "otaurl"
#define CFG_PARAM_PUSH_FERMENTRACK   "fermentrackpush"
#define CFG_PARAM_PUSH_BREWFATHER    "brewfatherpush"
#define CFG_PARAM_PUSH_HTTP          "httppush"
#define CFG_PARAM_PUSH_INTERVAL      "pushinterval"
#define CFG_PARAM_TEMPFORMAT         "tempformat"
#define CFG_PARAM_PRESSURECORR       "pressurecorr"
#define CFG_PARAM_VOLTAGEFACTOR      "voltagefactor"

class Config {
    private:
        bool   saveNeeded;

        char   id[10];
        char   mDNS[30];
        char   otaURL[200];
        char   fermentrackPushTarget[200];
        char   brewfatherPushTarget[200];
        char   httpPushTarget[200];
        char   tempFormat[2];
        char   pressureZeroCorrection[20];
        char   pushInterval[10];
        char   voltageFactor[20];

        void debug();
        void formatFileSystem();

    public:
        Config();
        const char*  getID() { return &id[0]; };

        const char*  getMDNS() { return &mDNS[0]; }
        void         setMDNS( const char* s ) { strncpy( &mDNS[0], s, sizeof(mDNS)-1); saveNeeded = true; }

        const char*  getOtaURL() { return &otaURL[0]; }
        void         setOtaURL( const char* s ) { strncpy( &otaURL[0], s, sizeof(otaURL)-1); saveNeeded = true; }
        bool         isOtaActive() { return strlen(&otaURL[0])>0?true:false; }

        const char*  getFermentrackPushTarget() { return &fermentrackPushTarget[0]; }
        void         setFermentrackPushTarget( const char* s ) { strncpy( &fermentrackPushTarget[0], s, sizeof(fermentrackPushTarget)-1); saveNeeded = true; }
        bool         isFermemtrackActive() { return strlen(&fermentrackPushTarget[0])>0?true:false; }

        const char*  getBrewfatherPushTarget() { return &brewfatherPushTarget[0]; }
        void         setBrewfatherPushTarget( const char* s ) { strncpy(&brewfatherPushTarget[0], s, sizeof(brewfatherPushTarget)-1); saveNeeded = true; }
        bool         isBrewfatherActive() { return strlen(&brewfatherPushTarget[0])>0?true:false; }

        const char*  getHttpPushTarget() { return &httpPushTarget[0]; }
        void         setHttpPushTarget( const char* s ) { strncpy(&httpPushTarget[0], s, sizeof(httpPushTarget)-1); saveNeeded = true; }
        bool         isHttpActive() { return strlen(&httpPushTarget[0])>0?true:false; }

        const char*  getPushInterval() { return &pushInterval[0]; }
        void         setPushInterval( const char* s ) { strncpy(&pushInterval[0], s, sizeof(pushInterval)-1); saveNeeded = true; }
        int          getPushIntervalAsInt() { return atoi(&pushInterval[0]); }

        const char*  getTempFormat() { return &tempFormat[0]; }
        void         setTempFormat( const char* s ) { strncpy( &tempFormat[0], s, sizeof(tempFormat)-1); saveNeeded = true; }
        bool         isTempC() { return strcmp( &tempFormat[0], "C")?false:true; };
        bool         isTempF() { return strcmp( &tempFormat[0], "F")?false:true; };

        const char*  getPressureZeroCorrection() { return &pressureZeroCorrection[0]; }
        void         setPressureZeroCorrection( const char* s ) { 
                convertFloatToString( atof(s),  &pressureZeroCorrection[0] ); 
                saveNeeded = true; 
            }
        float        getPressureZeroCorrectionAsFloat() { return atof(getPressureZeroCorrection()); }

        const char*  getVoltageFactor() { return &voltageFactor[0]; }
        void         setVoltageFactor( const char* s ) {
                convertFloatToString( atof(s), &voltageFactor[0]); 
                saveNeeded = true; 
            }
        float        getVoltageFactorAsFloat() { return atof(getVoltageFactor()); }

        // IO functions
        void createJson(StaticJsonDocument<512>& doc);
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