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

#include <baseconfig.hpp>
#include <main.hpp>

constexpr auto PARAM_PRESSURE_FORMAT = "pressure_format";
constexpr auto PARAM_PRESSURE_ZERO_CORRECTION = "pressure_zero_correction";
constexpr auto PARAM_PRESSURE_SENSOR_TYPE = "pressure_sensor_type";
constexpr auto PARAM_SENSOR_MIN_PRESSURE = "pressure_sensor_min";
constexpr auto PARAM_SENSOR_MAX_PRESSURE = "pressure_sensor_max";
constexpr auto PARAM_VOLTAGE_FACTOR = "voltage_factor";

constexpr auto PRESSURE_HPA = "hpa";
constexpr auto PRESSURE_BAR = "bar";
constexpr auto PRESSURE_PSI = "psi";

enum PressureSensorType {
  SensorHoneywell = 0,
};

class PressConfig : public BaseConfig {
 private:
  String _pressureFormat = PRESSURE_PSI;
  PressureSensorType _pressureSensor = PressureSensorType::SensorHoneywell;

  float _pressureZeroCorrection = 0;
  int _pressureSensorMin = 0;
  int _pressureSensorMax = 150;

  int _pushInterval = 300;
  float _voltageFactor = 1.59;

 public:
  PressConfig(String baseMDNS, String fileName);

  void createJson(JsonObject& doc);
  void parseJson(JsonObject& doc);
  void migrateSettings();

  const char* getPressureFormat() {
    return _pressureFormat.c_str();
  }
  String getPressureFormatAsString() {
    return _pressureFormat;
  }
  void setPressureFormat(String s) {
    _pressureFormat = s;
    _saveNeeded = true;
  }

  PressureSensorType getPressureSensorType() {
    return _pressureSensor;
  }
  int getPressureSensorTypeAsInt() {
    return (int) _pressureSensor;
  }
  void setPressureSensorType(int s) {
    _pressureSensor = (PressureSensorType) s;
    _saveNeeded = true;
  }

  float getPressureZeroCorrection() { return _pressureZeroCorrection; }
  void setPressureZeroCorrection(float v) {
    _pressureZeroCorrection = v;
    _saveNeeded = true;
  }

  float getVoltageFactor() { return _voltageFactor; }
  void setVoltageFactor(float v) {
    _voltageFactor = v;
    _saveNeeded = true;
  }

  int getPushInterval() { return _pushInterval; }
  void setPushInterval(int v) {
    _pushInterval = v;
    _saveNeeded = true;
  }

  int getPressureSensorMin() { return _pressureSensorMin; }
  void setPressureSensorMin(int v) {
    _pressureSensorMin = v;
    _saveNeeded = true;
  }

  int getPressureSensorMax() { return _pressureSensorMax; }
  void setPressureSensorMax(int v) {
    _pressureSensorMax = v;
    _saveNeeded = true;
  }
};

extern PressConfig myConfig;

#endif  // SRC_PRESSCONFIG_HPP_

// EOF
