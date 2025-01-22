/*
MIT License

Copyright (c) 2021-2024 Magnus

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
#ifndef SRC_CONFIG_HPP_
#define SRC_CONFIG_HPP_

#include <Arduino.h>

#include <main.hpp>

enum PressureSensorType {
  SensorNone = 0,
  
  SensorXidibeiXDB401_I2C_KPa_300 = 1,  // 0-0.3 MPa
  SensorXidibeiXDB401_I2C_KPa_400 = 2,  // 0-0.4 MPa
  SensorXidibeiXDB401_I2C_KPa_500 = 3,  // 0-0.5 MPa
  SensorXidibeiXDB401_I2C_KPa_600 = 4,  // 0-0.6 MPa

  SensorXidibeiXDB401_Analog_KPa_300 = 11,  // 0-03 MPa, 3.3V, 0.2 -> 2.4V
  SensorXidibeiXDB401_Analog_KPa_400 = 12,  // 0-04 MPa, 3.3V, 0.2 -> 2.4V
  SensorXidibeiXDB401_Analog_KPa_500 = 13,  // 0-05 MPa, 3.3V, 0.2 -> 2.4V
  SensorXidibeiXDB401_Analog_KPa_600 = 14,  // 0-06 MPa, 3.3V, 0.2 -> 2.4V
};

constexpr auto PRESSURE_HPA = "hpa";
constexpr auto PRESSURE_BAR = "bar";
constexpr auto PRESSURE_PSI = "psi";

constexpr auto MAX_SENSOR_DEVICES = 2;

class PressConfig {
 private:
  String _pressureUnit = PRESSURE_PSI;
  char _tempFormat = 'C';

  PressureSensorType _pressureSensor[MAX_SENSOR_DEVICES] = {
      PressureSensorType::SensorNone, PressureSensorType::SensorNone};

  // XIDIBEI Sensor specific settings
  float _sensorPressureCorrection[MAX_SENSOR_DEVICES] = {0, 0};
  float _sensorTemperatureCorrection[MAX_SENSOR_DEVICES] = {0, 0};

  bool _saveNeeded = false;

 public:
  PressConfig() {}

  char getTempFormat() { return _tempFormat; }

  const char* getPressureUnit() { return _pressureUnit.c_str(); }
  String getPressureUnitAsString() { return _pressureUnit; }
  void setPressureUnit(String s) {
    _pressureUnit = s;
    _saveNeeded = true;
  }

  PressureSensorType getPressureSensorType(int idx) {
    return _pressureSensor[idx];
  }
  int getPressureSensorTypeAsInt(int idx) {
    return static_cast<int>(_pressureSensor[idx]);
  }
  void setPressureSensorType(PressureSensorType s, int idx) {
    _pressureSensor[idx] = s;
    _saveNeeded = true;
  }
  void setPressureSensorType(int s, int idx) {
    setPressureSensorType((PressureSensorType)s, idx);
  }

  float getSensorPressureCorrection(int idx) { return _sensorPressureCorrection[idx]; }
  void setSensorPressureCorrection(float v, int idx) {
    _sensorPressureCorrection[idx] = v;
    _saveNeeded = true;
  }

  float getSensorTemperatureCorrection(int idx) { return _sensorTemperatureCorrection[idx]; }
  void setSensorTemperatureCorrection(float v, int idx) {
    _sensorTemperatureCorrection[idx] = v;
    _saveNeeded = true;
  }

  void saveFile() {}
};

extern PressConfig myConfig;

#endif  // SRC_CONFIG_HPP_

// EOF
