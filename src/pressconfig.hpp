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
#include <resources.hpp>

enum PressureSensorType {
  SensorHoneywellGaugePsi_30 = 0,
  SensorHoneywellGaugePsi_60 = 1,
  SensorHoneywellGaugePsi_100 = 2,
  SensorHoneywellGaugePsi_150 = 3,
  SensorCFSensorXGZP6847DGaugeKPa_700 = 10,   // 0 - 700 kPa
  SensorCFSensorXGZP6847DGaugeKPa_1000 = 11,  // -100 - 1000 kPa
};

class PressConfig : public BaseConfig {
 private:
  String _pressureUnit = PRESSURE_PSI;

  PressureSensorType _pressureSensor =
      PressureSensorType::SensorHoneywellGaugePsi_30;
  int _pressureSensorMin = 0;   // For Honeywell sensors
  int _pressureSensorMax = 30;  // For Honeywell sensors
  int _pressureSensorK = 0;     // For CFSensor sensors

  float _pressureZeroCorrection = 0;

  int _pushInterval = 300;
  float _voltageFactor = 1.59;

 public:
  PressConfig(String baseMDNS, String fileName);

  void createJson(JsonObject& doc);
  void parseJson(JsonObject& doc);
  void migrateSettings();

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

    switch (_pressureSensor) {
      case PressureSensorType::SensorHoneywellGaugePsi_30:
        _pressureSensorMin = 0;
        _pressureSensorMax = 30;
        break;

      case PressureSensorType::SensorHoneywellGaugePsi_60:
        _pressureSensorMin = 0;
        _pressureSensorMax = 60;
        break;

      case PressureSensorType::SensorHoneywellGaugePsi_100:
        _pressureSensorMin = 0;
        _pressureSensorMax = 100;
        break;

      case PressureSensorType::SensorHoneywellGaugePsi_150:
        _pressureSensorMin = 0;
        _pressureSensorMax = 150;
        break;

      case PressureSensorType::SensorCFSensorXGZP6847DGaugeKPa_700:
      case PressureSensorType::SensorCFSensorXGZP6847DGaugeKPa_1000:
        _pressureSensorK = 64;
        break;
    }

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
  int getPressureSensorMax() { return _pressureSensorMax; }
  int getPressureSensorK() { return _pressureSensorK; }
  int getVoltagePin() { return PIN_BATTERY; }
};

extern PressConfig myConfig;

#endif  // SRC_PRESSCONFIG_HPP_

// EOF
