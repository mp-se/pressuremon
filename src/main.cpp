/*
MIT License

Copyright (c) 2022-2024 Magnus

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
#include <Wire.h>
#include <pins_arduino.h>

#include <log.hpp>
#include <config.hpp>
#include <looptimer.hpp>
#include <main.hpp>
#include <pressure.hpp>

#include <XIDIBEI.hpp>

SerialDebug mySerial(115200L);
PressConfig myConfig;

void scanI2C(TwoWire *wire) {
  for (uint8_t i = 1; i < 128; i++) {
    wire->beginTransmission(i);
    if (wire->endTransmission() == 0) {
      Log.notice(F("Main : Found device at %X." CR), i);
    }
  }
}

void doDriverTest(TwoWire *wire) {
  int maxPressure = 4000;

  XIDIBEI sensor(maxPressure, wire);

  Log.notice(F("Test: Init sensor %s, maxPressure %d kPA." CR), sensor.begin() ? "ok" : "error", maxPressure);

  float a, b;

  while(1) {
    Log.notice(F("Test: Read data %s." CR), sensor.read(a, b) ? "ok" : "error");
    Log.notice(F("Test: Data %F kPA, %F bar, %F C." CR), a, convertPaPressureToBar(a*1000), b);
    delay(2000);
  }
}

// #define ENABLE_WIRE1 1
// #define ENABLE_DRIVER_TEST 1

void setup() {
  delay(2000);  // Allow for usbc serial to connect

  Log.notice(F("Main: Starting up." CR));

  int clock = 400000;

  Log.notice(F("Main: OneWire SDA=%d, SCL=%d." CR), SDA, SCL);
  Wire.setPins(SDA, SCL);
  Wire.begin();
  Wire.setClock(clock);

#if defined(ENABLE_DRIVER_TEST)
  doDriverTest(&Wire);
#endif

   Log.notice(F("Main: Scanning Wire. Clock=%d, Timeout=%d." CR),
             Wire.getClock(), Wire.getTimeOut());
   scanI2C(&Wire);

#if defined(ENABLE_WIRE1)
   Log.notice(F("Main: OneWire SDA1=%d, SCL1=%d." CR), RX, TX); 
   Wire1.setPins(RX, TX); 
   Wire1.begin(); 
   Wire1.setClock(clock);


  Log.notice(F("Main: Scanning Wire1. Clock=%d, Timeout=%d." CR), Wire1.getClock(), Wire1.getTimeOut()); 
  scanI2C(&Wire1);
#endif

  // ------------------------------------------------------------------------------------------------------------------------
  // Example with two separate Wire bus that allows for two separate sensors
  //

  myConfig.setPressureSensorType(
      PressureSensorType::SensorXidibeiXDB401_I2C_KPa_4000, 0);
  myPressureSensor[0].setup(0, &Wire);

  myPressureSensor[0].calibrate();

#if defined(ENABLE_WIRE1)
  myConfig.setPressureSensorType(
      PressureSensorType::SensorXidibeiXDB401_I2C_KPa_400, 1);
  myPressureSensor[1].setup(1, &Wire1);
#endif

  // ------------------------------------------------------------------------------------------------------------------------
  // Example with ADS1115 and 4 analog channels
  //

  /*myConfig.setPressureSensorType(
      PressureSensorType::SensorXidibeiXDB401_Analog_KPa_400, 0);
  myPressureSensor[0].setup(0, &Wire);*/

#if defined(ENABLE_WIRE1)
  myConfig.setPressureSensorType(
      PressureSensorType::SensorXidibeiXDB401_Analog_KPa_400, 1);
  myPressureSensor[1].setup(1, &Wire);
#endif

  Log.notice(F("Main: Setup completed." CR));
}

LoopTimer timer(2000);

void loop() {
  if (timer.hasExipred()) {
    timer.reset();

    myPressureSensor[0].read();
    Log.notice(F("Loop: Pressure 0 %F psi, Temp %F C, Voltage %F mV." CR),
               myPressureSensor[0].getPressurePsi(),
               myPressureSensor[0].getTemperatureC(),
               myPressureSensor[0].getAnalogVoltage());

#if defined(ENABLE_WIRE1)
    myPressureSensor[1].readSensor();
    Log.notice(F("Loop: Pressure 1 %F psi, Temp %F C, Voltage %F mV." CR),
    myPressureSensor[1].getPressurePsi(),
    myPressureSensor[1].getTemperatureC(),
    myPressureSensor[0].getAnalogVoltage());
#endif
  }
}

float convertPsiPressureToBar(float psi) { return psi * 0.0689475729; }

float convertPsiPressureToKPa(float psi) { return psi * 68.947572932 * 1000; }

float convertPaPressureToPsi(float pa) { return pa * 0.000145038; }

float convertPaPressureToBar(float pa) { return pa / 100000; }

// EOF
