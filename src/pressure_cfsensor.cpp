/*
MIT License

Copyright (c) 2024-2025 Magnus

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
#include <config.hpp>
#include <memory>
#include <pressure_cfsensor.hpp>

void CFSensorPressureSensor::setup(uint16_t k, uint8_t idx) {
  _zeroCorrection = myConfig.getCfZeroCorrection(idx);
  _idx = idx;
  _cfsensorSensor.reset(new XGZP6897D(k, &Wire));
  _sensorActive = _cfsensorSensor->begin();
  Log.notice(F("PRES: CFSensor sensor initialized %s, k = %d zero correction = "
               "%F (%d)" CR),
             _sensorActive ? "true" : "false", k, _zeroCorrection, _idx);
}

void CFSensorPressureSensor::calibrateSensor() {
  Log.notice(F("PRES: Starting auto calibration (%d)." CR), _idx);
  float zero = 0;

  for (int i = 0; i < 10; i++) {
    loop();
    float f = getPressurePsi(false);
    Log.notice(F("PRES: Step %d, Pressure = %F (%d)." CR), i + 1, f, _idx);
    zero += f;
    delay(500);
  }

  Log.notice(F("PRES: Measured difference %F (%d)." CR), zero / 10, _idx);
  myConfig.setCfZeroCorrection(zero / 10, _idx);
  myConfig.saveFile();
  _zeroCorrection = myConfig.getCfZeroCorrection(_idx);
}

float CFSensorPressureSensor::getTemperatureC() { return _temperature; }

float CFSensorPressureSensor::getPressurePsi(bool doCorrection) {
  if (doCorrection) return _pressure - _zeroCorrection;
  return _pressure;
}

bool CFSensorPressureSensor::readSensor() {
  float pressure;

  // Returns temperature in C and pressure in Pa
  bool b = _cfsensorSensor->readSensor(_temperature, pressure);
  _pressure = convertPaPressureToPsi(pressure);
  return b;
}

// EOF
