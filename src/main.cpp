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

#include <config.hpp>
#include <looptimer.hpp>
#include <main.hpp>
#include <pressure.hpp>

SerialDebug mySerial(115200L);
PressConfig myConfig;
PressureSensor myPressureSensor[MAX_SENSOR_DEVICES];

void scanI2C(TwoWire *wire) {
  for (uint8_t i = 1; i < 127; i++) {
    wire->beginTransmission(i);
    if (wire->endTransmission() == 0) {
      Log.notice(F("Main : Found device at %X." CR), i);
    }
  }
}

void setup() {
  // delay(5000);  // Allow for usbc serial to connect

  Log.notice(F("Main: Starting up." CR));

  int clock = 100000;

  Log.notice(F("Main: OneWire SDA=%d, SCL=%d." CR), SDA, SCL);
  Wire.setPins(SDA, SCL);
  Wire.begin();
  // Wire.setClock(clock);

  // Log.notice(F("Main: OneWire SDA1=%d, SCL1=%d, SDA2=%d, SCL2=%d ." CR), SDA,
  // SCL, RX, TX); Wire1.setPins(RX, TX); Wire1.begin(); Wire1.setClock(clock);

  Log.notice(F("Main: Scanning Wire. Clock=%d, Timeout=%d." CR),
             Wire.getClock(), Wire.getTimeOut());
  scanI2C(&Wire);

  // Log.notice(F("Main: Scanning Wire1. Clock=%d, Timeout=%d." CR),
  // Wire1.getClock(), Wire1.getTimeOut()); scanI2C(&Wire1);

  // ------------------------------------------------------------------------------------------------------------------------
  // Example with two separate Wire bus that allows for two separate sensors
  //

  /*myConfig.setPressureSensorType(
      PressureSensorType::SensorXidibeiXDB401_I2C_KPa_400, 0);
  myPressureSensor[0].setup(0, &Wire);

  myConfig.setPressureSensorType(
      PressureSensorType::SensorXidibeiXDB401_I2C_KPa_400, 1);
  // myPressureSensor[1].setup(1, &Wire1);
  */

  // ------------------------------------------------------------------------------------------------------------------------
  // Example with ADS1115 and 4 analog channels
  //

  myConfig.setPressureSensorType(
      PressureSensorType::SensorXidibeiXDB401_Analog_KPa_400, 0);
  myPressureSensor[0].setup(0, &Wire);
  myConfig.setPressureSensorType(
      PressureSensorType::SensorXidibeiXDB401_Analog_KPa_400, 1);
  // myPressureSensor[1].setup(1, &Wire);

  Log.notice(F("Main: Setup completed." CR));
}

LoopTimer timer(2000);

void loop() {
  if (timer.hasExipred()) {
    timer.reset();

    myPressureSensor[0].readSensor();
    Log.notice(F("Loop: Pressure 0 %F psi, Temp %F C, Voltage %F mV." CR),
               myPressureSensor[0].getPressurePsi(),
               myPressureSensor[0].getTemperatureC(),
               myPressureSensor[0].getAnalogVoltage());
    // myPressureSensor[1].readSensor();
    // Log.notice(F("Loop: Pressure 1 %F psi, Temp %F C, Voltage %F mV." CR),
    // myPressureSensor[1].getPressurePsi(),
    // myPressureSensor[1].getTemperatureC(),
    // myPressureSensor[0].getAnalogVoltage());
  }
}

// EOF
