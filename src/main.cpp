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
#include "helper.h"
#include "config.h"
#include "wifi.h"
#include "webserver.h"
#include "pushtarget.h"
#include "pressuresensor.h"

// Settings for double reset detector.
#define ESP8266_DRD_USE_RTC       true
#define DRD_TIMEOUT               2
#define DRD_ADDRESS               0
#define DOUBLERESETDETECTOR_DEBUG true
#include <ESP_DoubleResetDetector.h>            
DoubleResetDetector *drd;

// Define constats for this program
const int interval = 5000;                  // ms, time to wait between changes to output
unsigned long lastMillis = 0;
unsigned long startMillis;
bool sleepModeActive = false;
bool sleepModeAlwaysSkip = false;           // Web interface can override normal behaviour

//
// Check if we should be in sleep mode
//
void checkSleepMode( float psi, float volt ) {
#if defined( SKIP_SLEEPMODE )
    sleepModeActive = false;
    Log.verbose(F("MAIN: Skipping sleep mode (SKIP_SLEEPMODE is defined)." CR) );
    return;
#endif

    if( sleepModeAlwaysSkip ) {
        Log.notice(F("MAIN: Sleep mode disabled from web interface." CR) );
        sleepModeActive = false;
        return;
    }

    // Will not enter sleep mode if: charger is connected 
    sleepModeActive = volt<4.15 ? true : false; 
#if LOG_LEVEL==6
    Log.verbose(F("MAIN: Deep sleep mode %s (psi=%F, volt=%F)." CR), sleepModeActive ? "true":"false", psi, volt );
#endif
}

//
// Main setup 
//
void setup() {
  // Record the starting time
    startMillis = millis();

#if defined( ACTIVATE_WIFI )
    drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS); // Timeout, Address
    bool dt = drd->detectDoubleReset();  
#endif

    // Initialize pin outputs
    Log.notice(F("Main: Started setup for %s." CR), String( ESP.getChipId(), HEX).c_str() );
    printBuildOptions();
    powerLedOn();
    Log.notice(F("Main: Loading configuration." CR));
    myConfig.checkFileSystem();
    myConfig.loadFile();

    // Setup watchdog
    ESP.wdtDisable();
    ESP.wdtEnable( interval*2 );

#if defined( ACTIVATE_WIFI )
  if( dt ) 
    Log.notice(F("Main: Detected doubletap on reset." CR));

  myWifi.connect( dt );
  
  if( myWifi.isConnected() )
    Log.notice(F("Main: Connected to wifi ip=%s." CR), myWifi.getIPAddress().c_str() );
#endif

    // Activate the pressure sensor and check the value
    myPressureSensor.setup();
    myPressureSensor.loop();  
    myBatteryVoltage.read();
    checkSleepMode( myPressureSensor.getPressurePsi(), myBatteryVoltage.getVoltage());

#if defined( ACTIVATE_WIFI ) && defined( ACTIVATE_OTA ) 
    if( !sleepModeActive && myWifi.isConnected() && myWifi.checkFirmwareVersion() ) {
        delay(500);
        myWifi.updateFirmware();
    }
#endif

#if defined( ACTIVATE_WIFI ) 
    if( myWifi.isConnected() ) {
        Log.notice(F("Main: Connected to wifi ip=%s." CR), myWifi.getIPAddress().c_str() );

  #if defined( ACTIVATE_OTA ) 
        if( !sleepModeActive && myWifi.checkFirmwareVersion() ) {
            myWifi.updateFirmware();
        }
  #endif
        if( !sleepModeActive )
            if( myWebServer.setupWebServer() )
                Log.notice(F("Main: Webserver is running." CR) );
    }
#endif

  powerLedOff();
}

//
// Main loop
//
void loop() {
    drd->loop();

    if( sleepModeActive || abs(millis() - lastMillis) > interval ) {
        float psi  = myPressureSensor.getPressurePsi();
        float temp = myPressureSensor.getTemperatureC();

        if( myConfig.isTempF() )
            temp = myPressureSensor.getTemperatureF();

#if LOG_LEVEL==6
        Log.verbose(F("MAIN: Pressure = %F psi, Temperature = %F %c." CR), psi, temp, myConfig.getTempFormat() );
#endif

#if defined( ACTIVATE_PUSH )
        myPushTarget.send( psi, temp, sleepModeActive );    // Force the transmission if we are going to sleep
#endif

        if( sleepModeActive ) {
            unsigned long runTime = millis() - startMillis;

            // Enter sleep mode...
            Log.notice(F("MAIN: Entering deep sleep, run time %l s." CR), runTime/1000 );
            drd->stop();
            delay(500);
            deepSleep( myConfig.getPushInterval() ); 
        }

        // Do these checks if we are running in normal mode (not sleep mode)
        checkSleepMode( psi, myBatteryVoltage.getVoltage() );
        myPressureSensor.loop();
        myBatteryVoltage.read();
        lastMillis = millis();
    }

    myWebServer.loop();
}

// EOF