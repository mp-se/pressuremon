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
#include <batteryvoltage.hpp>
#include <main.hpp>
#include <pressureconfig.hpp>
#include <pressuresensor.hpp>
#include <utils.hpp>

constexpr auto PARAM_PLATFORM = "platform";

PressureConfig::PressureConfig(String baseMDNS, String fileName)
    : BaseConfig(baseMDNS, fileName, JSON_BUFFER) {}

void PressureConfig::createJson(DynamicJsonDocument& doc, bool skipSecrets) {
  // Call base class functions
  createJsonBase(doc, skipSecrets);
  createJsonWifi(doc, skipSecrets);
  createJsonOta(doc, skipSecrets);
  createJsonPush(doc, skipSecrets);

#if defined(ESP32)
  doc[PARAM_PLATFORM] = "esp32";
#elif defined(ESP32S2)
  doc[PARAM_PLATFORM] = "esp32s2";
#endif

  // Handle project specific config
  doc[PARAM_PUSH_INTERVAL] = getPushInterval();
  doc[PARAM_SENSOR_MIN_PRESSURE] = getSensorMinPressure();
  doc[PARAM_SENSOR_MAX_PRESSURE] = getSensorMaxPressure();
  doc[PARAM_PRESSURE_ZERO_CORRECTION] =
      serialized(String(getPressureZeroCorrection(), DECIMALS_PRESSURE));
  doc[PARAM_PRESSURE_FORMAT] = getPressureFormatTypeAsString();
  doc[PARAM_PUSH_FORMAT] = getPushFormatTypeAsInt();
  doc[PARAM_VOLTAGE_FACTOR] =
      serialized(String(getVoltageFactor(), DECIMALS_BATTERY));

  // Add data that is displayed on config
  doc[PARAM_PRESSURE] =
      serialized(String(myPressureSensor.getPressure(true), DECIMALS_PRESSURE));
  doc[PARAM_TEMP] = serialized(
      String(myPressureSensor.getTemperature(), DECIMALS_TEMPERATURE));
  doc[PARAM_BATTERY] =
      serialized(String(myBatteryVoltage.getVoltage(), DECIMALS_BATTERY));
}

void PressureConfig::parseJson(DynamicJsonDocument& doc) {
  // Call base class functions
  parseJsonBase(doc);
  parseJsonWifi(doc);
  parseJsonOta(doc);
  parseJsonPush(doc);

  // Handle project specific config
  if (!doc[PARAM_PUSH_INTERVAL].isNull())
    setPushInterval(doc[PARAM_PUSH_INTERVAL].as<int>());

  if (!doc[PARAM_SENSOR_MIN_PRESSURE].isNull())
    setSensorMinPressure(doc[PARAM_SENSOR_MIN_PRESSURE].as<int>());
  if (!doc[PARAM_SENSOR_MAX_PRESSURE].isNull())
    setSensorMaxPressure(doc[PARAM_SENSOR_MAX_PRESSURE].as<int>());

  if (!doc[PARAM_PRESSURE_ZERO_CORRECTION].isNull())
    setPressureZeroCorrection(doc[PARAM_PRESSURE_ZERO_CORRECTION].as<float>());

  if (!doc[PARAM_PRESSURE_FORMAT].isNull())
    setPressureFormatTypeAsString(doc[PARAM_PRESSURE_FORMAT]);

  if (!doc[PARAM_PUSH_FORMAT].isNull())
    setPushFormat(doc[PARAM_PUSH_FORMAT].as<int>());

  if (!doc[PARAM_VOLTAGE_FACTOR].isNull())
    setVoltageFactor(doc[PARAM_VOLTAGE_FACTOR].as<float>());
}

// EOF
