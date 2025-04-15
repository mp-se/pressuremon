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
 
  SensorXidibeiXDB401_I2C_KPa_200 = 1,  // 0-0.2 MPa
  SensorXidibeiXDB401_I2C_KPa_400 = 2,  // 0-0.4 MPa
  SensorXidibeiXDB401_I2C_KPa_500 = 3,  // 0-0.5 MPa
  SensorXidibeiXDB401_I2C_KPa_600 = 4,  // 0-0.6 MPa
  SensorXidibeiXDB401_I2C_KPa_800 = 5,  // 0-0.8 MPa
  SensorXidibeiXDB401_I2C_KPa_1000 = 6,  // 0-1 MPa
  SensorXidibeiXDB401_I2C_KPa_1200 = 7,  // 0-1.2 MPa
  SensorXidibeiXDB401_I2C_KPa_1500 = 8,  // 0-1.5 MPa
  SensorXidibeiXDB401_I2C_KPa_1600 = 9,  // 0-1.6 MPa
  SensorXidibeiXDB401_I2C_KPa_2000 = 10,  // 0-2 MPa
  SensorXidibeiXDB401_I2C_KPa_2500 = 11,  // 0-2.5 MPa
  SensorXidibeiXDB401_I2C_KPa_3000 = 12,  // 0-3 MPa
  SensorXidibeiXDB401_I2C_KPa_3500 = 13,  // 0-3.5 MPa
  SensorXidibeiXDB401_I2C_KPa_4000 = 15,  // 0-4 MPa

  SensorXidibeiXDB401_Analog_KPa_200 = 101,  // 0-0.2 MPa
  SensorXidibeiXDB401_Analog_KPa_400 = 102,  // 0-0.4 MPa
  SensorXidibeiXDB401_Analog_KPa_500 = 103,  // 0-0.5 MPa
  SensorXidibeiXDB401_Analog_KPa_600 = 104,  // 0-0.6 MPa
  SensorXidibeiXDB401_Analog_KPa_800 = 105,  // 0-0.8 MPa
  SensorXidibeiXDB401_Analog_KPa_1000 = 106,  // 0-1 MPa
  SensorXidibeiXDB401_Analog_KPa_1200 = 107,  // 0-1.2 MPa
  SensorXidibeiXDB401_Analog_KPa_1500 = 108,  // 0-1.5 MPa
  SensorXidibeiXDB401_Analog_KPa_1600 = 109,  // 0-1.6 MPa
  SensorXidibeiXDB401_Analog_KPa_2000 = 110,  // 0-2 MPa
  SensorXidibeiXDB401_Analog_KPa_2500 = 111,  // 0-2.5 MPa
  SensorXidibeiXDB401_Analog_KPa_3000 = 112,  // 0-3 MPa
  SensorXidibeiXDB401_Analog_KPa_3500 = 113,  // 0-3.5 MPa
  SensorXidibeiXDB401_Analog_KPa_4000 = 115,  // 0-4 MPa
};

constexpr auto PRESSURE_KPA = "kPa";
constexpr auto PRESSURE_BAR = "Bar";
constexpr auto PRESSURE_PSI = "PSI";

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

  bool isPressureBar() { return _pressureUnit == PRESSURE_BAR; }
  bool isPressureKpa() { return _pressureUnit == PRESSURE_KPA; }
  bool isPressurePsi() { return _pressureUnit == PRESSURE_PSI; }

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

  float getPressureSensorCorrection(int idx) {
    return _sensorPressureCorrection[idx];
  }
  void setPressureSensorCorrection(float v, int idx) {
    _sensorPressureCorrection[idx] = v;
    _saveNeeded = true;
  }

  float getTemperatureSensorCorrection(int idx) {
    return _sensorTemperatureCorrection[idx];
  }
  void setTemperatureSensorCorrection(float v, int idx) {
    _sensorTemperatureCorrection[idx] = v;
    _saveNeeded = true;
  }

  void saveFile() {}
};

extern PressConfig myConfig;

#endif  // SRC_CONFIG_HPP_

// EOF
