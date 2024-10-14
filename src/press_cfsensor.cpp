/*
MIT License

Copyright (c) 2024 Magnus

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
#include <pressconfig.hpp>
#include <pressure.hpp>

void CFSensorPressureSensor::setup() {
  _zeroCorrection = myConfig.getPressureZeroCorrection();
  _cfsensorSensor = new XGZP6897D(myConfig.getPressureSensorK(), &Wire);
  bool status = _cfsensorSensor->begin();
  Log.notice(
      F("PRES: CFSensor sensor initialized %s, zero correction = %F" CR), status ? "true" : "false", _zeroCorrection);
}

void CFSensorPressureSensor::loop() {
}

void CFSensorPressureSensor::calibrateSensor() {
  Log.notice(F("PRES: Starting auto calibration." CR));
  float zero = 0;

  for (int i = 0; i < 10; i++) {
    loop();
    float f = getPressurePsi(false);
    Log.notice(F("PRES: Step %d, Pressure = %F." CR), i + 1, f);
    zero += f;
    delay(500);
  }

  Log.notice(F("PRES: Measured difference %F." CR), zero / 10);
  myConfig.setPressureZeroCorrection(zero / 10);
  myConfig.saveFile();
  _zeroCorrection = myConfig.getPressureZeroCorrection();
}

float CFSensorPressureSensor::getTemperatureC() {
  return _temperature;
}

float CFSensorPressureSensor::getPressurePsi(bool doCorrection) {
  float pressure;

  // Returns temperature in C and pressure in Pa
  _cfsensorSensor->readSensor(_temperature, pressure);
  _pressure = convertPressure2Psi(pressure);

  if (doCorrection) return _pressure - _zeroCorrection;
  return _pressure;
}

// EOF
