/*
MIT License

Copyright (c) 2021-2025 Magnus

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
#ifndef SRC_PRESSURE_HPP_
#define SRC_PRESSURE_HPP_

#include <Arduino.h>
#include <log.hpp>

#include <memory>
class PressureSensorInterface {
  public:
  PressureSensorInterface() = default;
  virtual void loop();

  virtual bool isSensorActive();
  virtual float getPressurePsi(bool doCorrection = true);
  virtual float getTemperatureC();
  virtual void calibrateSensor();
};

class PressureSensor {
 private:
  std::unique_ptr<PressureSensorInterface> _impl;

 public:
  void setup();
  void loop() { if(_impl) loop(); }
  bool isSensorActive() { return _impl == nullptr ? false : _impl->isSensorActive(); }

  float getPressurePsi(bool doCorrection = true) { return _impl == nullptr ? NAN : _impl->getPressurePsi(doCorrection); }
  float getTemperatureC() { return _impl == 0 ? NAN : _impl->getTemperatureC(); }
  float getTemperatureF() { return (getTemperatureC() * 1.8) + 32.0; }

  void calibrateSensor() { if(_impl) _impl->calibrateSensor(); }

  // When reading the values always read pressure first and then temperature. 
  // Some sensors have one way of getting the data which is implemented in 
  // the getPressure method.

  float getPressure(bool doCorrection = true); // Returns in chosen device format
  float getTemperature(); // Returns in chosen device format
};

float convertPsiPressureToBar(float psi);
float convertPsiPressureToHPa(float psi);

float convertPaPressureToPsi(float pa);

#endif  // SRC_PRESSURE_HPP_

// EOF
