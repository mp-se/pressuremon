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
  // CFSensor
  SensorCFSensorXGZP6847DGaugeKPa_700 = 200,   // 0 - 700 kPa
  SensorCFSensorXGZP6847DGaugeKPa_1000 = 201,  // -100 - 1000 kPa
  // XIDIBEI
  SensorXidibeiXDB401_KPa_300 = 300,  // 0-0.3 MPa
  SensorXidibeiXDB401_KPa_400 = 301,  // 0-0.4 MPa
  SensorXidibeiXDB401_KPa_500 = 302,  // 0-0.5 MPa
  SensorXidibeiXDB401_KPa_600 = 303,  // 0-0.6 MPa
};

constexpr auto PRESSURE_HPA = "hpa";
constexpr auto PRESSURE_BAR = "bar";
constexpr auto PRESSURE_PSI = "psi";

constexpr auto MAX_PRESSURE_DEVICES = 8;

class PressConfig {
 private:
  String _pressureUnit = PRESSURE_PSI;
  char _tempFormat = 'C';

  PressureSensorType _pressureSensor[MAX_PRESSURE_DEVICES] = {
      PressureSensorType::SensorNone, PressureSensorType::SensorNone,
      PressureSensorType::SensorNone, PressureSensorType::SensorNone,
      PressureSensorType::SensorNone, PressureSensorType::SensorNone,
      PressureSensorType::SensorNone, PressureSensorType::SensorNone};

  // Honeywell specific settings
  float _honeywellZeroCorrection[MAX_PRESSURE_DEVICES] = {0, 0, 0, 0,
                                                          0, 0, 0, 0};

  // CF Sensor specific settings
  float _cfZeroCorrection[MAX_PRESSURE_DEVICES] = {0, 0, 0, 0, 0, 0, 0, 0};

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

  float getHoneywellZeroCorrection(int idx) {
    return _honeywellZeroCorrection[idx];
  }
  void setHoneywellZeroCorrection(float v, int idx) {
    _honeywellZeroCorrection[idx] = v;
    _saveNeeded = true;
  }

  float getCfZeroCorrection(int idx) { return _cfZeroCorrection[idx]; }
  void setCfZeroCorrection(float v, int idx) {
    _cfZeroCorrection[idx] = v;
    _saveNeeded = true;
  }

  void saveFile() {}
};

extern PressConfig myConfig;

#endif  // SRC_CONFIG_HPP_

// EOF
