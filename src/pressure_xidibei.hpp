/*
MIT License

Copyright (c) 2025 Magnus

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
#ifndef SRC_PRESSURE_XIDIBEI_HPP_
#define SRC_PRESSURE_XIDIBEI_HPP_

#include <pressure.hpp>

#include <XIDIBEI.hpp>

class XIDIBEIPressureSensor : public PressureSensorInterface {
  private:
    std::unique_ptr<XIDIBEI> _xidibeiSensor;
    float _zeroCorrection = 0;
    bool _sensorActive = false;
    float _pressure, _temperature;

  public:
    XIDIBEIPressureSensor() {}

    void setup(float maxPressure);
    void loop();

    bool isSensorActive() { return _sensorActive; }
    float getPressurePsi(bool doCorrection = true);
    float getTemperatureC();
    void calibrateSensor();
};

#endif  // SRC_PRESSURE_XIDIBEI_HPP_

// EOF
