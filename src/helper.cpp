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
#include <Ticker.h>  

SerialDebug    stirSerial;
BatteryVoltage myBatteryVoltage;

Ticker powerLedBlinker;
bool   powerLedConfigured = false;

//
// Activate the ticker function to flash the led
//
void activateLedTicker(float t) {
    powerLedBlinker.attach( t, powerLedToggle );
}

//
// Disable the ticker function
//
void deactivateLedTicker() {
    powerLedBlinker.detach();
    powerLedOff();
}

//
// Setup the LED pin as output
//
void powerLedConfigure() {
    if( !powerLedConfigured ) {
        pinMode( LED_BUILTIN, OUTPUT );
        powerLedConfigured = true;
    }
}

//
// Turn on power led 
//
void powerLedOn() {
    powerLedConfigure();    
    digitalWrite( LED_BUILTIN, 0);
}

//
// Turn off the power led
//
void powerLedOff() {
    powerLedConfigure();    
    digitalWrite( LED_BUILTIN, 1);
}

//
// Toggle the power led
//
void powerLedToggle() {
    powerLedConfigure();    
    digitalWrite( LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

//
// Enter deep sleep for the defined duration (Argument is seconds)
//
void deepSleep(int t) {
#if LOG_LEVEL==6
    Log.verbose(F("HELP: Entering sleep mode for %ds." CR), t );
#endif
    uint64_t wake = t * 1000000;
    ESP.deepSleep( wake );
}

//
// Print the build options used
//
void printBuildOptions() {
    Log.notice( F("Build options: LOGLEVEL %d %s " 
#ifdef SIMULATE_SENSOR
                "SIMULATE "
#endif    
#ifdef ACTIVATE_WIFI
                "WIFI "
#endif    
#ifdef ACTIVATE_OTA
                "OTA "
#endif    
#ifdef ACTIVATE_PUSH
                "PUSH "
#endif    
#ifdef DISPLAY_SELFTEST    
                "DISPLAY_SELFTEST "
#endif    
    CR), LOG_LEVEL, CFG_APPVER );
}

//
// Configure serial debug output
//
SerialDebug::SerialDebug(const long serialSpeed) { 
    // Start serial with auto-detected rate (default to defined BAUD)
    Serial.flush();
    Serial.begin(serialSpeed);

    getLog()->begin(LOG_LEVEL, &Serial, true);
    getLog()->setPrefix(printTimestamp);
    getLog()->notice(F("SDBG: Serial logging started at %l." CR), serialSpeed);
}

//
// Print the timestamp (ms since start of device)
//
void printTimestamp(Print* _logOutput) {
  char c[12];
  sprintf(c, "%10lu ", millis());
  _logOutput->print(c);
}

//
// Read and calculate the battery voltage
//
void BatteryVoltage::read() { 
    // The analog pin can only handle 3.3V maximum voltage so we need to reduce the voltage (from max 5V)
    float factor = myConfig.getVoltageFactorAsFloat();      // Default value is 1.63 
    int v = analogRead( A0 );
    batteryLevel = ((3.3/1023)*v)*factor;
#if LOG_LEVEL==6
    Log.verbose(F("BATT: Reading voltage level. Factor=%F Value=%d, Voltage=%F." CR), factor, v, batteryLevel );
#endif
}

//
// Convert float to formatted string with 2 decimals
//
void convertFloatToString( float f, char *buffer ) {
    dtostrf(f, 6, 2, buffer); 
}

//
// Reduce precision to 2 decimals
//
float reduceFloatPrecision( float f ) {
    char buffer[10];
    dtostrf(f, 6, 2, &buffer[0]); 
    return atof(&buffer[0]);
}

// EOF 