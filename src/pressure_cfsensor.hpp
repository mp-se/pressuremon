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
#ifndef SRC_PRESSURE_CFSENSOR_HPP_
#define SRC_PRESSURE_CFSENSOR_HPP_

#include <XGZP6897D.hpp>
#include <memory>
#include <pressure.hpp>

// CFSensor pinout (from top). Pin 2 is below the notch in the sensor.
//
//  6 5 4
//  1 ^ 3
//
// 1 = SCL
// 2 = NC
// 3 = GND
// 4 = VCC
// 5 = SDA
// 6 = NC
//
class CFSensorPressureSensor : public PressureSensorInterface {
 private:
  std::unique_ptr<XGZP6897D> _cfsensorSensor;
  uint8_t _idx;
  float _zeroCorrection = 0;
  float _pressure, _temperature;
  bool _sensorActive = false;

 public:
  CFSensorPressureSensor() {}

  void setup(uint16_t k, uint8_t idx);
  bool readSensor();

  bool isSensorActive() { return _sensorActive; }
  float getPressurePsi(bool doCorrection = true);
  float getTemperatureC();
  void calibrateSensor();
};

#endif  // SRC_PRESSURE_CFSENSOR_HPP_

// EOF
