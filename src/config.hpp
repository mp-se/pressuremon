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
#ifndef SRC_PRESSCONFIG_HPP_
#define SRC_PRESSCONFIG_HPP_

#include <Arduino.h>
#include <main.hpp>

enum PressureSensorType {
  // Honewywell
  SensorHoneywellGaugePsi_30 = 0,
  SensorHoneywellGaugePsi_60 = 1,
  SensorHoneywellGaugePsi_100 = 2,
  SensorHoneywellGaugePsi_150 = 3,
  // CFSensor
  SensorCFSensorXGZP6847DGaugeKPa_700 = 100,   // 0 - 700 kPa
  SensorCFSensorXGZP6847DGaugeKPa_1000 = 101,  // -100 - 1000 kPa
  // XIDIBEI
  SensorXidibeiXDB401_KPa_300 = 200, // 0-0.3 MPa
  SensorXidibeiXDB401_KPa_400 = 201, // 0-0.4 MPa
  SensorXidibeiXDB401_KPa_500 = 202, // 0-0.5 MPa
  SensorXidibeiXDB401_KPa_600 = 203, // 0-0.6 MPa
};

constexpr auto PRESSURE_HPA = "hpa";
constexpr auto PRESSURE_BAR = "bar";
constexpr auto PRESSURE_PSI = "psi";

class PressConfig {
 private:
  String _pressureUnit = PRESSURE_PSI;
  char _tempFormat = 'C';

  PressureSensorType _pressureSensor = PressureSensorType::SensorHoneywellGaugePsi_30;

  // Honeywell specific settings
  float _honeywellZeroCorrection = 0;

  // CF Sensor specific settings
  float _cfZeroCorrection = 0;

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

  PressureSensorType getPressureSensorType() { return _pressureSensor; }
  int getPressureSensorTypeAsInt() { return static_cast<int>(_pressureSensor); }
  void setPressureSensorType(int s) {
    _pressureSensor = (PressureSensorType)s;
    _saveNeeded = true;
  }

  float getHoneywellZeroCorrection() { return _honeywellZeroCorrection; }
  void setHoneywellZeroCorrection(float v) {
    _honeywellZeroCorrection = v;
    _saveNeeded = true;
  }

  float getCfZeroCorrection() { return _cfZeroCorrection; }
  void setCfZeroCorrection(float v) {
    _cfZeroCorrection = v;
    _saveNeeded = true;
  }

  void saveFile() {}
};

extern PressConfig myConfig;

#endif  // SRC_PRESSCONFIG_HPP_

// EOF