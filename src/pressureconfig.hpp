/*
MIT License

Copyright (c) 2021-23 Magnus

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
#ifndef SRC_PRESSURECONFIG_HPP_
#define SRC_PRESSURECONFIG_HPP_

#include <baseconfig.hpp>
#include <main.hpp>

constexpr auto PARAM_PUSH_INTERVAL = "push-interval";
constexpr auto PARAM_PRESSURE_ZERO_CORRECTION = "pressure-zero-correction";
constexpr auto PARAM_PRESSURE_FORMAT = "pressure-format";
constexpr auto PARAM_SENSOR_MIN_PRESSURE = "sensor-min-pressure";
constexpr auto PARAM_SENSOR_MAX_PRESSURE = "sensor-max-pressure";
constexpr auto PARAM_PUSH_FORMAT = "push-format";
constexpr auto PARAM_PRESSURE = "pressure";
constexpr auto PARAM_TEMP = "temp";
constexpr auto PARAM_BATTERY = "battery";
constexpr auto PARAM_VOLTAGE_FACTOR = "voltage-factor";

constexpr auto DECIMALS_PRESSURE = 4;
constexpr auto DECIMALS_TEMPERATURE = 2;
constexpr auto DECIMALS_BATTERY = 2;

enum PressureFormatType { Psi = 0, Bar = 1, hPA = 2 };

enum PushFormatType { Default = 0, Brewfather = 1 };

constexpr auto PRESSURE_PSI = "psi";
constexpr auto PRESSURE_BAR = "bar";
constexpr auto PRESSURE_HPA = "hpa";

class PressureConfig : public BaseConfig {
 private:
  PressureFormatType _pressureFormat = PressureFormatType::Psi;
  PushFormatType _pushFormat = PushFormatType::Default;
  float _pressureZeroCorrection;
  int _sensorMinPressure = 0;
  int _sensorMaxPressure = 150;
  int _pushInterval = 300;
  float _voltageFactor = 1.59;

 public:
  PressureConfig(String baseMDNS, String fileName);

  void createJson(DynamicJsonDocument& doc, bool skipSecrets = true);
  void parseJson(DynamicJsonDocument& doc);

  const char* getPressureFormatTypeAsString() {
    switch (_pressureFormat) {
      case PressureFormatType::Psi:
        return PRESSURE_PSI;
      case PressureFormatType::Bar:
        return PRESSURE_BAR;
      case PressureFormatType::hPA:
        return PRESSURE_HPA;
    }
    return "";
  }
  void setPressureFormatTypeAsString(String s) {
    if (s.equalsIgnoreCase(PRESSURE_BAR))
      _pressureFormat = PressureFormatType::Bar;
    else if (s.equalsIgnoreCase(PRESSURE_HPA))
      _pressureFormat = PressureFormatType::hPA;
    else
      _pressureFormat = PressureFormatType::Psi;
    _saveNeeded = true;
  }
  PressureFormatType getPressureFormatType() { return _pressureFormat; }
  void setPressureFormatType(PressureFormatType v) {
    _pressureFormat = v;
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

  int getPushFormatTypeAsInt() { return _pushFormat; }
  PushFormatType getPushFormatType() { return _pushFormat; }
  void setPushFormat(int v) {
    _pushFormat = (PushFormatType)v;
    _saveNeeded = true;
  }

  int getPushInterval() { return _pushInterval; }
  void setPushInterval(int v) {
    _pushInterval = v;
    _saveNeeded = true;
  }

  int getSensorMinPressure() { return _sensorMinPressure; }
  void setSensorMinPressure(int v) {
    _sensorMinPressure = v;
    _saveNeeded = true;
  }

  int getSensorMaxPressure() { return _sensorMaxPressure; }
  void setSensorMaxPressure(int v) {
    _sensorMaxPressure = v;
    _saveNeeded = true;
  }
};

extern PressureConfig myConfig;

#endif  // SRC_PRESSURECONFIG_HPP_

// EOF
