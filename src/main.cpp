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
#include <i2c_mux.hpp>
#include <looptimer.hpp>
#include <main.hpp>
#include <pressure.hpp>

SerialDebug mySerial(115200L);
PressConfig myConfig;
PressureSensor myPressureSensor[MAX_PRESSURE_DEVICES];
I2CMux myMux;

void scanI2C() {
  for (uint8_t i = 1, j = 0; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Log.notice(F("Main : Found device at %X." CR), i);
      j++;
    }
  }
}

void setup() {
  delay(5000);  // Allow for usbc serial to connect

  Log.notice(F("Main: Starting up." CR));
  Log.notice(F("Main: OneWire SDA=%d, SCL=%d." CR), SDA, SCL);

  Wire.begin(SDA, SCL);

  bool b = myMux.begin(&Wire);
  // scanI2C();

  /*for(int i = 0; i < MAX_PRESSURE_DEVICES; i ++) {
    Log.notice(F("Main: Scanning bus %d." CR), i);
    myMux.selectBus(i);
    scanI2C();
  }*/

  // This is just for testing and evaluating the sensors
  // Code base will be merged into the gravitymon project to enable sharing of
  // the common code and make maintenance easier

  myConfig.setPressureSensorType(
      PressureSensorType::SensorCFSensorXGZP6847DGaugeKPa_700, 0);
  myConfig.setPressureSensorType(
      PressureSensorType::SensorCFSensorXGZP6847DGaugeKPa_700, 1);
  myPressureSensor[0].setup(0, &myMux);
  myPressureSensor[1].setup(1, &myMux);
  Log.notice(F("Main: Setup completed." CR));
}

LoopTimer timer(2000);

void loop() {
  if (timer.hasExipred()) {
    timer.reset();

    for(int i = 0; i < MAX_PRESSURE_DEVICES; i++ ) {
      if(myConfig.getPressureSensorType(i) != PressureSensorType::SensorNone) {
        myPressureSensor[i].readSensor();
        Log.notice(F("Loop: Pressure %d %F psi, Temp %F C." CR), i, myPressureSensor[i].getPressurePsi(), myPressureSensor[i].getTemperatureC());
      }
    }
  }
}

// EOF
