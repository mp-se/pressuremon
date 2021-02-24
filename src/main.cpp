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
#include "numled.h"
#include "wifi.h"
#include "pushtarget.h"
#include "pressuresensor.h"

// Settings for double reset
#define ESP8266_DRD_USE_RTC true
#define DOUBLERESETDETECTOR_DEBUG true
#include <ESP_DoubleResetDetector.h>

// Define constats for this program
const int interval = 5000;                  // ms, time to wait between changes to output
unsigned long lastMillis = 0;
unsigned long startMillis;
bool sleepModeActive = false;
DoubleResetDetector* myDRD;

//
// Check if we should be in sleep mode
//
void checkSleepMode( float psi, float volt ) {
#if defined( SKIP_SLEEPMODE )
  sleepModeActive = false;
  //Log.verbose(F("MAIN: Skipping sleep mode (SKIP_SLEEPMODE is defined)." CR) );
  return;
#endif

  // If there is pressure on the sensor and the power is less than 4.5V but higher than 2.5V (Assume on battery)
  // we will then go into deep sleep mode.
  sleepModeActive = ( abs(psi) > 0.5 ) && ( volt < 4.5 ) && ( volt > 2.5 ) ? true : false; 
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

  // Initialize pin outputs
  Log.notice(F("Main: Started setup for %s." CR), String( ESP.getChipId(), HEX).c_str() );
  printBuildOptions();
  powerLedOn();
  myDRD = new DoubleResetDetector(2, 0); // Timeout, Address
#if defined( ACTIVATE_WIFI )
  bool dt = myDRD->detectDoubleReset();  
#endif
  myNumLED.setup();

  Log.notice(F("Main: Loading configuration." CR));
  myConfig.checkFileSystem();
  myConfig.loadFile();

  // Setup watchdog
  ESP.wdtDisable();
  ESP.wdtEnable( interval*2 );

#if defined( ACTIVATE_WIFI )
  if( dt ) 
    Log.notice(F("Main: Detected doubletap on reset." CR));

  myNumLED.printConn();
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
    myNumLED.printUpd();
    myWifi.updateFirmware();
  }
#endif

  powerLedOff();
}

//
// Main loop
//
void loop() {
  myDRD->loop();

  if( sleepModeActive || (millis() - lastMillis) > interval ) {
    float psi  = myPressureSensor.getPressurePsi();
    float temp = myPressureSensor.getTemperatureC();

    if( myConfig.isTempF() ) {
#if LOG_LEVEL==6
      Log.verbose(F("MAIN: Using Farenheigt for temperature." CR) );
#endif
      temp = myPressureSensor.convertTemperature2F( temp );
    }

#if LOG_LEVEL==6
    Log.verbose(F("MAIN: Pressure = %F psi, Temperature = %F %s." CR), psi, temp, myConfig.getTempFormat() );
#endif

    if( myPressureSensor.isSensorActive() )
      myNumLED.printNum( psi );
    else
      myNumLED.printDash(4);

#if defined( ACTIVATE_PUSH )
    myPushTarget.send( psi, temp, sleepModeActive );    // Force the transmission if we are going to sleep
#endif

    if( sleepModeActive ) {
      unsigned long runTime = millis() - startMillis;

      // Enter sleep mode...
      Log.notice(F("MAIN: Entering deep sleep, run time %l s." CR), runTime/1000 );
      delay(500);
      deepSleep( myConfig.getPushIntervalAsInt() ); 
    }

    // Do these checks if we are running in normal mode (not sleep mode)
    checkSleepMode( psi, myBatteryVoltage.getVoltage() );
    myPressureSensor.loop();
    myBatteryVoltage.read();
    lastMillis = millis();
  }

  myWifi.loop();
}

// EOF