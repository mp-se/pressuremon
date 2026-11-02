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
#include <helper.hpp>
#include <main.hpp>
#include <pressure.hpp>
#include <push_pressuremon.hpp>
#include <pushtarget.hpp>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <cmath>

void setupTemplateEnginePressure(PressuremonConfig* config,
                                 TemplatingEngine& engine, float pressurePsi,
                                 float pressurePsi1, float tempC, float runTime,
                                 float voltage) {
  const float rawPressurePsi = pressurePsi;
  const float rawPressurePsi1 = pressurePsi1;
  const float rawTempC = tempC;

  // Preserve the legacy values for existing placeholders.
  if (!std::isfinite(pressurePsi)) pressurePsi = 0;
  if (!std::isfinite(pressurePsi1)) pressurePsi1 = 0;
  if (!std::isfinite(tempC)) tempC = 0;

  const auto setNullValue = [&engine](const char* token, float value,
                                      int decimals) {
    if (std::isfinite(value))
      engine.setVal(token, value, decimals);
    else
      engine.setVal(token, "null");
  };

  // Names
  engine.setVal(TPL_MDNS, config->getMDNS());
  engine.setVal(TPL_ID, config->getID());
  engine.setVal(TPL_TOKEN, config->getToken());
  engine.setVal(TPL_TOKEN2, config->getToken2());

  // Temperature
  if (config->isTempFormatC()) {
    engine.setVal(TPL_TEMP, tempC, DECIMALS_TEMP);
    setNullValue(TPL_TEMP_NULL, rawTempC, DECIMALS_TEMP);
  } else {
    const float tempF = convertCtoF(tempC);
    const float rawTempF = convertCtoF(rawTempC);
    engine.setVal(TPL_TEMP, tempF, DECIMALS_TEMP);
    setNullValue(TPL_TEMP_NULL, rawTempF, DECIMALS_TEMP);
  }

  engine.setVal(TPL_TEMP_C, tempC, DECIMALS_TEMP);
  engine.setVal(TPL_TEMP_F, convertCtoF(tempC), DECIMALS_TEMP);
  engine.setVal(TPL_TEMP_UNITS, config->getTempFormat());

  // Battery & Timer
  engine.setVal(TPL_BATTERY, voltage, DECIMALS_BATTERY);
  engine.setVal(TPL_SLEEP_INTERVAL, config->getSleepInterval());

  engine.setVal(TPL_BATTERY_PERCENT,
                getBatteryPercentage(voltage, config->getBatteryType()), 1);

  // Performance metrics
  engine.setVal(TPL_RUN_TIME, runTime, DECIMALS_RUNTIME);
  engine.setVal(TPL_RSSI, WiFi.RSSI());

  engine.setVal(TPL_PRESSURE_PSI, pressurePsi, DECIMALS_PRESSURE);
  engine.setVal(TPL_PRESSURE1_PSI, pressurePsi1, DECIMALS_PRESSURE);
  engine.setVal(TPL_PRESSURE_BAR, convertPsiPressureToBar(pressurePsi),
                DECIMALS_PRESSURE);
  engine.setVal(TPL_PRESSURE1_BAR, convertPsiPressureToBar(pressurePsi1),
                DECIMALS_PRESSURE);
  engine.setVal(TPL_PRESSURE_KPA, convertPsiPressureToKPa(pressurePsi),
                DECIMALS_PRESSURE);
  engine.setVal(TPL_PRESSURE_KPA, convertPsiPressureToKPa(pressurePsi1),
                DECIMALS_PRESSURE);

  if (config->isPressureBar()) {
    const float pressureBar = convertPsiPressureToBar(pressurePsi);
    const float pressure1Bar = convertPsiPressureToBar(pressurePsi1);
    engine.setVal(TPL_PRESSURE, pressureBar, DECIMALS_PRESSURE);
    engine.setVal(TPL_PRESSURE1, pressure1Bar, DECIMALS_PRESSURE);
    setNullValue(TPL_PRESSURE_NULL, convertPsiPressureToBar(rawPressurePsi), DECIMALS_PRESSURE);
    setNullValue(TPL_PRESSURE1_NULL, convertPsiPressureToBar(rawPressurePsi1), DECIMALS_PRESSURE);
  } else if (config->isPressureKpa()) {
    const float pressureKpa = convertPsiPressureToKPa(pressurePsi);
    const float pressure1Kpa = convertPsiPressureToKPa(pressurePsi1);
    engine.setVal(TPL_PRESSURE, pressureKpa, DECIMALS_PRESSURE);
    engine.setVal(TPL_PRESSURE1, pressure1Kpa, DECIMALS_PRESSURE);
    setNullValue(TPL_PRESSURE_NULL, convertPsiPressureToKPa(rawPressurePsi), DECIMALS_PRESSURE);
    setNullValue(TPL_PRESSURE1_NULL, convertPsiPressureToKPa(rawPressurePsi1), DECIMALS_PRESSURE);
  } else {
    engine.setVal(TPL_PRESSURE, pressurePsi, DECIMALS_PRESSURE);
    engine.setVal(TPL_PRESSURE1, pressurePsi1, DECIMALS_PRESSURE);
    setNullValue(TPL_PRESSURE_NULL, rawPressurePsi, DECIMALS_PRESSURE);
    setNullValue(TPL_PRESSURE1_NULL, rawPressurePsi1, DECIMALS_PRESSURE);
  }

  engine.setVal(TPL_PRESSURE_UNIT, config->getPressureUnit());

  engine.setVal(TPL_APP_VER, CFG_APPVER);
  engine.setVal(TPL_APP_BUILD, CFG_GITREV);
}

#endif  // PRESSUREMON

// EOF
