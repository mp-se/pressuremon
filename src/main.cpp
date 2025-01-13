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
PressureSensor myPressureSensor[MAX_SENSOR_DEVICES];
I2CMux myMux;

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

  // ------------------------------------------------------------------------------------------------------------------------
  // Example with multiplexer that allows for up to 8 sensors
  //

  /*
  Log.notice(F("Main: OneWire SDA=%d, SCL=%d." CR), SDA, SCL);
  Wire.setPins(SDA, SCL);
  Wire.begin();
  Wire.setClock(400000);
  Log.notice(F("Main: Scanning I2C bus. Clock=%d, Timeout=%d" CR), Wire.getClock(), Wire.getTimeOut());

  bool b = myMux.begin(&Wire);

  for(int i = 0; i < MAX_PRESSURE_DEVICES; i ++) {
    Log.notice(F("Main: Scanning I2C bus %d." CR), i);
    myMux.selectBus(i);
    scanI2C(&Wire);
  }

  myConfig.setPressureSensorType(
      PressureSensorType::SensorCFSensorXGZP6847DGaugeKPa_700, 0);
  myConfig.setPressureSensorType(
      PressureSensorType::SensorCFSensorXGZP6847DGaugeKPa_700, 1);
  myPressureSensor[0].setup(0, &Wire, &myMux);
  myPressureSensor[1].setup(1, &Wire, &myMux);
  */

  // ------------------------------------------------------------------------------------------------------------------------
  // Example with two separate Wire bus that allows for two separate sensors
  //

  Log.notice(F("Main: OneWire SDA1=%d, SCL1=%d, SDA2=%d, SCL2=%d ." CR), SDA, RX, TX);
  Wire.setPins(SDA, SCL);
  Wire.begin();
  Wire.setClock(400000);

  Wire1.setPins(RX, TX);
  Wire1.begin();
  Wire1.setClock(400000);

  Log.notice(F("Main: Scanning I2C bus. Clock1=%d, Timeout1=%d, Clock2=%d, Timeout2=%d." CR), Wire.getClock(), Wire.getTimeOut(), Wire1.getClock(), Wire1.getTimeOut());

  Log.notice(F("Main: Scanning I2C bus 1." CR));
  scanI2C(&Wire);

  Log.notice(F("Main: Scanning I2C bus 2." CR));
  scanI2C(&Wire1);

  myConfig.setPressureSensorType(
      PressureSensorType::SensorXidibeiXDB401_KPa_300, 0);
  myPressureSensor[0].setup(0, &Wire);

  myConfig.setPressureSensorType(
      PressureSensorType::SensorXidibeiXDB401_KPa_300, 1);
  myPressureSensor[1].setup(1, &Wire1);


 Log.notice(F("Main: Setup completed." CR));
}

LoopTimer timer(2000);

void loop() {
  if (timer.hasExipred()) {
    timer.reset();

    // ------------------------------------------------------------------------------------------------------------------------
    // Example with multiplexer that allows for up to 8 sensors
    //

    /*
    for(int i = 0; i < MAX_PRESSURE_DEVICES; i++ ) {
      if(myConfig.getPressureSensorType(i) != PressureSensorType::SensorNone) {
        myPressureSensor[i].readSensor();
        Log.notice(F("Loop: Pressure %d %F psi, Temp %F C." CR), i, myPressureSensor[i].getPressurePsi(), myPressureSensor[i].getTemperatureC());
      }
    }
    */

    // ------------------------------------------------------------------------------------------------------------------------
    // Example with two separate Wire bus that allows for two separate sensors
    //

    myPressureSensor[0].readSensor();
    Log.notice(F("Loop: Pressure 0 %F psi, Temp %F C." CR), myPressureSensor[0].getPressurePsi(), myPressureSensor[0].getTemperatureC());
    myPressureSensor[1].readSensor();
    Log.notice(F("Loop: Pressure 1 %F psi, Temp %F C." CR), myPressureSensor[1].getPressurePsi(), myPressureSensor[1].getTemperatureC());

  }
}

// EOF
