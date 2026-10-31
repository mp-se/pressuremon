/*
 * PressureMon
 * Copyright (c) 2021-2026 Magnus
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Alternatively, this software may be used under the terms of a
 * commercial license. See LICENSE_COMMERCIAL for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef SRC_CONFIG_PRESSUREMON_HPP_
#define SRC_CONFIG_PRESSUREMON_HPP_

#if defined(PRESSUREMON)

#include <config_brewing.hpp>
#include <main_pressuremon.hpp>
#include <pressure.hpp>

enum PressuremonBleFormat {
  BLE_DISABLED = 0,
  BLE_PRESSUREMON_IBEACON = 1,
  // BLE_PRESSUREMON_EDDYSTONE = 2
};

constexpr auto MAX_SENSOR_DEVICES = 2;

constexpr auto PRESSURE_KPA = "kPa";
constexpr auto PRESSURE_BAR = "Bar";
constexpr auto PRESSURE_PSI = "PSI";

constexpr auto CONFIG_SENSOR_TYPE = "sensor_type";
constexpr auto CONFIG_SENSOR1_TYPE = "sensor1_type";
constexpr auto CONFIG_PRESSURE_ADJUSTMENT = "pressure_adjustment";
constexpr auto CONFIG_PRESSURE1_ADJUSTMENT = "pressure1_adjustment";

constexpr auto CONFIG_PRESSURE_UNIT = "pressure_unit";

constexpr auto CONFIG_CUSTOM_MIN_VOLTAGE = "custom_min_voltage";
constexpr auto CONFIG_CUSTOM_MAX_VOLTAGE = "custom_max_voltage";
constexpr auto CONFIG_CUSTOM_MIN_PRESSURE = "custom_min_pressure";
constexpr auto CONFIG_CUSTOM_MAX_PRESSURE = "custom_max_pressure";

constexpr auto CONFIG_CUSTOM1_MIN_VOLTAGE = "custom1_min_voltage";
constexpr auto CONFIG_CUSTOM1_MAX_VOLTAGE = "custom1_max_voltage";
constexpr auto CONFIG_CUSTOM1_MIN_PRESSURE = "custom1_min_pressure";
constexpr auto CONFIG_CUSTOM1_MAX_PRESSURE = "custom1_max_pressure";

struct AnalogCustomSensor {
  float minPressure;  // kPa
  float maxPressure;  // kPa
  float minVoltage;
  float maxVoltage;
};

class PressuremonConfig : public BrewingConfig, public PressureConfigInterface {
 private:
  PressuremonBleFormat _pressuremonBleFormat =
      PressuremonBleFormat::BLE_DISABLED;
  PressureSensorType _pressureSensor[MAX_SENSOR_DEVICES] = {
      PressureSensorType::SensorNone, PressureSensorType::SensorNone};

  float _pressureSensorCorrection[MAX_SENSOR_DEVICES] = {0, 0};
  AnalogCustomSensor _analogCustomSensor[MAX_SENSOR_DEVICES] = {{0, 0, 0, 0},
                                                                {0, 0, 0, 0}};

  bool _batterySaving = true;

  String _pressureUnit = PRESSURE_PSI;

 public:
  PressuremonConfig(String baseMDNS, String fileName);

  int getVoltagePin() const { return PIN_VOLT; }

  bool isPressureBar() const { return _pressureUnit == PRESSURE_BAR; }
  bool isPressureKpa() const { return _pressureUnit == PRESSURE_KPA; }
  bool isPressurePsi() const { return _pressureUnit == PRESSURE_PSI; }

  const char* getPressureUnit() const { return _pressureUnit.c_str(); }
  void setPressureUnit(String s) {
    _pressureUnit = s;
    _saveNeeded = true;
  }

  PressureSensorType getPressureSensorType(int idx) const {
    return _pressureSensor[idx];
  }
  int getPressureSensorTypeAsInt(int idx) const {
    return static_cast<int>(_pressureSensor[idx]);
  }
  void setPressureSensorType(PressureSensorType t, int idx) {
    _pressureSensor[idx] = t;
    _saveNeeded = true;
  }
  void setPressureSensorType(int t, int idx) {
    setPressureSensorType((PressureSensorType)t, idx);
  }
  bool hasPressureSensorConfigured() const {
    return (_pressureSensor[0] != PressureSensorType::SensorNone ||
            _pressureSensor[1] != PressureSensorType::SensorNone);
  }

  float getPressureSensorCorrection(int idx) const {
    return _pressureSensorCorrection[idx];
  }
  void setPressureSensorCorrection(float v, int idx) {
    _pressureSensorCorrection[idx] = v;
    _saveNeeded = true;
  }

  float getCustomAnalogMinV(int idx) const {
    return _analogCustomSensor[idx].minVoltage;
  }
  void setCustomAnalogMinV(float v, int idx) {
    _analogCustomSensor[idx].minVoltage = v;
    _saveNeeded = true;
  }
  float getCustomAnalogMaxV(int idx) const {
    return _analogCustomSensor[idx].maxVoltage;
  }
  void setCustomAnalogMaxV(float v, int idx) {
    _analogCustomSensor[idx].maxVoltage = v;
    _saveNeeded = true;
  }
  float getCustomAnalogMinKpa(int idx) const {
    return _analogCustomSensor[idx].minPressure;
  }
  void setCustomAnalogMinKpa(float p, int idx) {
    _analogCustomSensor[idx].minPressure = p;
    _saveNeeded = true;
  }
  float getCustomAnalogMaxKpa(int idx) const {
    return _analogCustomSensor[idx].maxPressure;
  }
  void setCustomAnalogMaxKpa(float p, int idx) {
    _analogCustomSensor[idx].maxPressure = p;
    _saveNeeded = true;
  }

  float getTemperatureSensorCorrection(int idx) const { return 0; }

  bool isBleActive() const {
    return (_pressuremonBleFormat != PressuremonBleFormat::BLE_DISABLED);
  }

  const PressuremonBleFormat getPressuremonBleFormat() const {
    return _pressuremonBleFormat;
  }
  void setPressuremonBleFormat(int b) {
    _pressuremonBleFormat = (PressuremonBleFormat)b;
    _saveNeeded = true;
  }
  void setPressuremonBleFormat(PressuremonBleFormat b) {
    _pressuremonBleFormat = b;
    _saveNeeded = true;
  }

  const bool isBatterySaving() const { return _batterySaving; }
  void setBatterySaving(bool b) {
    _batterySaving = b;
    _saveNeeded = true;
  }

  void createJson(JsonObject& doc) const;
  void parseJson(JsonObject& doc);

  // Wrappers for PressureConfig
  char getTempUnit() const { return BaseConfig::getTempUnit(); }
  bool saveFile() { return BaseConfig::saveFile(); }
};

extern PressuremonConfig myConfig;

#endif  // GRAVITYMON

#endif  // SRC_CONFIG_PRESSUREMON_HPP_

// EOF
