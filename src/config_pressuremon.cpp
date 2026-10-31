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
#if defined(PRESSUREMON)

#include <config_pressuremon.hpp>
#include <main.hpp>

PressuremonConfig::PressuremonConfig(String baseMDNS, String fileName)
    : BrewingConfig(baseMDNS, fileName) {
#if defined(ESP32C3)
  setVoltageFactor(2.41);  // Override default voltage factor for C3 pico board
#endif
}

void PressuremonConfig::createJson(JsonObject& doc) const {
  BrewingConfig::createJson(doc);

  doc[CONFIG_BATTERY_SAVING] = isBatterySaving();

  doc[CONFIG_PRESSURE_UNIT] = getPressureUnit();
  doc[CONFIG_BLE_FORMAT] = getPressuremonBleFormat();

  doc[CONFIG_SENSOR_TYPE] = getPressureSensorTypeAsInt(0);
  doc[CONFIG_SENSOR1_TYPE] = getPressureSensorTypeAsInt(1);
  doc[CONFIG_PRESSURE_ADJUSTMENT] =
      serialized(String(getPressureSensorCorrection(0), DECIMALS_PRESSURE));
  doc[CONFIG_PRESSURE1_ADJUSTMENT] =
      serialized(String(getPressureSensorCorrection(1), DECIMALS_PRESSURE));

  doc[CONFIG_CUSTOM_MIN_VOLTAGE] =
      serialized(String(getCustomAnalogMinV(0), 3));
  doc[CONFIG_CUSTOM_MAX_VOLTAGE] =
      serialized(String(getCustomAnalogMaxV(0), 3));
  doc[CONFIG_CUSTOM_MIN_PRESSURE] =
      serialized(String(getCustomAnalogMinKpa(0), DECIMALS_PRESSURE));
  doc[CONFIG_CUSTOM_MAX_PRESSURE] =
      serialized(String(getCustomAnalogMaxKpa(0), DECIMALS_PRESSURE));

  doc[CONFIG_CUSTOM1_MIN_VOLTAGE] =
      serialized(String(getCustomAnalogMinV(1), 3));
  doc[CONFIG_CUSTOM1_MAX_VOLTAGE] =
      serialized(String(getCustomAnalogMaxV(1), 3));
  doc[CONFIG_CUSTOM1_MIN_PRESSURE] =
      serialized(String(getCustomAnalogMinKpa(1), DECIMALS_PRESSURE));
  doc[CONFIG_CUSTOM1_MAX_PRESSURE] =
      serialized(String(getCustomAnalogMaxKpa(1), DECIMALS_PRESSURE));
}

void PressuremonConfig::parseJson(JsonObject& doc) {
  BrewingConfig::parseJson(doc);

  if (!doc[CONFIG_BATTERY_SAVING].isNull())
    setBatterySaving(doc[CONFIG_BATTERY_SAVING].as<bool>());

  if (!doc[CONFIG_PRESSURE_UNIT].isNull())
    setPressureUnit(doc[CONFIG_PRESSURE_UNIT].as<String>());

  if (!doc[CONFIG_BLE_FORMAT].isNull())
    setPressuremonBleFormat(doc[CONFIG_BLE_FORMAT].as<int>());

  if (!doc[CONFIG_SENSOR_TYPE].isNull())
    setPressureSensorType(doc[CONFIG_SENSOR_TYPE].as<int>(), 0);
  if (!doc[CONFIG_SENSOR1_TYPE].isNull())
    setPressureSensorType(doc[CONFIG_SENSOR1_TYPE].as<int>(), 1);

  if (!doc[CONFIG_PRESSURE_ADJUSTMENT].isNull())
    setPressureSensorCorrection(doc[CONFIG_PRESSURE_ADJUSTMENT].as<float>(), 0);
  if (!doc[CONFIG_PRESSURE1_ADJUSTMENT].isNull())
    setPressureSensorCorrection(doc[CONFIG_PRESSURE1_ADJUSTMENT].as<float>(),
                                1);

  if (!doc[CONFIG_CUSTOM_MIN_VOLTAGE].isNull())
    setCustomAnalogMinV(doc[CONFIG_CUSTOM_MIN_VOLTAGE].as<float>(), 0);
  if (!doc[CONFIG_CUSTOM_MAX_VOLTAGE].isNull())
    setCustomAnalogMaxV(doc[CONFIG_CUSTOM_MAX_VOLTAGE].as<float>(), 0);
  if (!doc[CONFIG_CUSTOM_MIN_PRESSURE].isNull())
    setCustomAnalogMinKpa(doc[CONFIG_CUSTOM_MIN_PRESSURE].as<float>(), 0);
  if (!doc[CONFIG_CUSTOM_MAX_PRESSURE].isNull())
    setCustomAnalogMaxKpa(doc[CONFIG_CUSTOM_MAX_PRESSURE].as<float>(), 0);

  if (!doc[CONFIG_CUSTOM1_MIN_VOLTAGE].isNull())
    setCustomAnalogMinV(doc[CONFIG_CUSTOM1_MIN_VOLTAGE].as<float>(), 1);
  if (!doc[CONFIG_CUSTOM1_MAX_VOLTAGE].isNull())
    setCustomAnalogMaxV(doc[CONFIG_CUSTOM1_MAX_VOLTAGE].as<float>(), 1);
  if (!doc[CONFIG_CUSTOM1_MIN_PRESSURE].isNull())
    setCustomAnalogMinKpa(doc[CONFIG_CUSTOM1_MIN_PRESSURE].as<float>(), 1);
  if (!doc[CONFIG_CUSTOM1_MAX_PRESSURE].isNull())
    setCustomAnalogMaxKpa(doc[CONFIG_CUSTOM1_MAX_PRESSURE].as<float>(), 1);
}

#endif  // PRESSUREMON

// EOF
