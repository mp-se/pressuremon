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
#include <log.hpp>
#include <main.hpp>
#include <pressconfig.hpp>
#include <utils.hpp>

PressConfig::PressConfig(String baseMDNS, String fileName)
    : BaseConfig(baseMDNS, fileName, JSON_BUFFER) {}

void PressConfig::createJson(JsonObject& doc) {
  // Call base class functions
  createJsonBase(doc);
  createJsonWifi(doc);
  // createJsonOta(doc);
  createJsonPush(doc);

  // Handle project specific config
  doc[PARAM_PRESSURE_UNIT] = getPressureUnit();
  doc[PARAM_PRESSURE_SENSOR_TYPE] = getPressureSensorTypeAsInt();

  doc[PARAM_PRESSURE_ZERO_CORRECTION] =
      serialized(String(getPressureZeroCorrection(), 4));
  doc[PARAM_VOLTAGE_FACTOR] = serialized(String(getVoltageFactor(), 2));
}

void PressConfig::parseJson(JsonObject& doc) {
  // Call base class functions
  parseJsonBase(doc);
  parseJsonWifi(doc);
  // parseJsonOta(doc);
  parseJsonPush(doc);

  // Handle project specific config
  if (!doc[PARAM_PRESSURE_UNIT].isNull())
    setPressureUnit(doc[PARAM_PRESSURE_UNIT].as<String>());
  if (!doc[PARAM_PRESSURE_SENSOR_TYPE].isNull())
    setPressureSensorType(doc[PARAM_PRESSURE_SENSOR_TYPE].as<int>());
  if (!doc[PARAM_PRESSURE_ZERO_CORRECTION].isNull())
    setPressureZeroCorrection(doc[PARAM_PRESSURE_ZERO_CORRECTION].as<float>());
  if (!doc[PARAM_VOLTAGE_FACTOR].isNull())
    setVoltageFactor(doc[PARAM_VOLTAGE_FACTOR].as<float>());
}

// EOF
