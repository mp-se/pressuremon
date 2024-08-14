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
#include <MQTT.h>

#include <main.hpp>
#include <pressconfig.hpp>
#include <presspush.hpp>
#include <templating.hpp>

// Use iSpindle format for compatibility, HTTP POST
const char httpPostFormat[] PROGMEM =
    "{"
    "\"name\": \"${mdns}\", "
    "\"id\": \"${id}\", "
    "\"interval\": ${sleep-interval}, "
    "\"temp\": ${temp}, "
    "\"temp_unit\": \"${temp-unit}\", "
    "\"pressure\": ${pressure}, "
    "\"pressure_unit\": ${pressure-unit}, "
    "\"battery\": ${battery}, "
    "\"rssi\": ${rssi}"
    "}";

// Format for an HTTP GET
const char httpGetFormat[] PROGMEM =
    "?name=${mdns}"
    "&id=${id}"
    "&interval=${sleep-interval}"
    "&temp=${temp}"
    "&temp_unit=${temp-unit}"
    "&pressure=${pressure}"
    "&pressure_unit=${pressure-unit}"
    "&battery=${battery}"
    "&rssi=${rssi}";

const char influxDbFormat[] PROGMEM =
    "measurement,host=${mdns},device=${id},temp_unit=${temp-unit},pressure-"
    "unit=${pressure-unit} "
    "pressure=${pressure},temp=${"
    "temp},battery=${battery},"
    "rssi=${rssi}\n";

const char mqttFormat[] PROGMEM =
    "pressmon/${mdns}/temp:${temp}|"
    "pressmon/${mdns}/temp_unit:${temp-unit}|"
    "pressmon/${mdns}/battery:${battery}|"
    "pressmon/${mdns}/pressure:${pressure}|"
    "pressmon/${mdns}/pressure_unit:${pressure-unit}|"
    "pressmon/${mdns}/interval:${sleep-interval}|"
    "pressmon/${mdns}/rssi:${rssi}|";

PressPush::PressPush(PressConfig* pressureConfig) : BasePush(pressureConfig) {
  _pressConfig = pressureConfig;
}

void PressPush::sendAll(float pressure, float temperature, float voltage) {
  _http.setReuse(true);
  _httpSecure.setReuse(true);

  TemplatingEngine engine;
  setupTemplateEngine(engine, pressure, temperature, voltage);

  if (myConfig.hasTargetHttpPost()) {
    String tpl = getTemplate(PressPush::TEMPLATE_HTTP1);
    String doc = engine.create(tpl.c_str());
    sendHttpPost(doc);
  }

  if (myConfig.hasTargetHttpPost2()) {
    String tpl = getTemplate(PressPush::TEMPLATE_HTTP2);
    String doc = engine.create(tpl.c_str());
    sendHttpPost2(doc);
  }

  if (myConfig.hasTargetHttpGet()) {
    String tpl = getTemplate(PressPush::TEMPLATE_HTTP3);
    String doc = engine.create(tpl.c_str());
    sendHttpGet(doc);
  }

  if (myConfig.hasTargetInfluxDb2()) {
    String tpl = getTemplate(PressPush::TEMPLATE_INFLUX);
    String doc = engine.create(tpl.c_str());
    sendInfluxDb2(doc);
  }

  if (myConfig.hasTargetMqtt()) {
    String tpl = getTemplate(PressPush::TEMPLATE_MQTT);
    String doc = engine.create(tpl.c_str());
    sendMqtt(doc);
  }

  engine.freeMemory();
}

const char* PressPush::getTemplate(Templates t, bool useDefaultTemplate) {
  String fname;
  _baseTemplate.reserve(600);

  // Load templates from memory
  switch (t) {
    case TEMPLATE_HTTP1:
      _baseTemplate = String(httpPostFormat);
      fname = TPL_FNAME_POST;
      break;
    case TEMPLATE_HTTP2:
      _baseTemplate = String(httpPostFormat);
      fname = TPL_FNAME_POST2;
      break;
    case TEMPLATE_HTTP3:
      _baseTemplate = String(httpGetFormat);
      fname = TPL_FNAME_GET;
      break;
    case TEMPLATE_INFLUX:
      _baseTemplate = String(influxDbFormat);
      fname = TPL_FNAME_INFLUXDB;
      break;
    case TEMPLATE_MQTT:
      _baseTemplate = String(mqttFormat);
      fname = TPL_FNAME_MQTT;
      break;
  }

  if (!useDefaultTemplate) {
    File file = LittleFS.open(fname, "r");
    if (file) {
      char buf[file.size() + 1];
      memset(&buf[0], 0, file.size() + 1);
      file.readBytes(&buf[0], file.size());
      _baseTemplate = String(&buf[0]);
      file.close();
      Log.notice(F("PUSH: Template loaded from disk %s." CR), fname.c_str());
    }
  }

#if LOG_LEVEL == 6
  Log.verbose(F("TPL : Base '%s'." CR), _baseTemplate.c_str());
#endif

  return _baseTemplate.c_str();
}

void PressPush::setupTemplateEngine(TemplatingEngine& engine, float pressure,
                                    float tempC, float voltage) {
  // Names
  engine.setVal(TPL_MDNS, myConfig.getMDNS());
  engine.setVal(TPL_ID, myConfig.getID());

  // Temperature
  if (myConfig.isTempFormatC()) {
    engine.setVal(TPL_TEMP, tempC, DECIMALS_TEMP);
  } else {
    engine.setVal(TPL_TEMP, convertCtoF(tempC), DECIMALS_TEMP);
  }

  engine.setVal(TPL_TEMP_C, tempC, DECIMALS_TEMP);
  engine.setVal(TPL_TEMP_F, convertCtoF(tempC), DECIMALS_TEMP);
  engine.setVal(TPL_TEMP_UNITS, myConfig.getTempUnit());

  // Battery & Timer
  engine.setVal(TPL_BATTERY, voltage, DECIMALS_BATTERY);
  engine.setVal(TPL_SLEEP_INTERVAL, myConfig.getPushInterval());

  int charge = 0;

  if (voltage > 4.15)
    charge = 100;
  else if (voltage > 4.05)
    charge = 90;
  else if (voltage > 3.97)
    charge = 80;
  else if (voltage > 3.91)
    charge = 70;
  else if (voltage > 3.86)
    charge = 60;
  else if (voltage > 3.81)
    charge = 50;
  else if (voltage > 3.78)
    charge = 40;
  else if (voltage > 3.76)
    charge = 30;
  else if (voltage > 3.73)
    charge = 20;
  else if (voltage > 3.67)
    charge = 10;
  else if (voltage > 3.44)
    charge = 5;

  engine.setVal(TPL_BATTERY_PERCENT, charge);

  // Performance metrics
  engine.setVal(TPL_RSSI, WiFi.RSSI());

  // Angle/Tilt
  engine.setVal(TPL_PRESSURE, pressure, DECIMALS_PRESSURE);
  engine.setVal(TPL_PRESSURE_PSI, pressure, DECIMALS_PRESSURE);
  engine.setVal(TPL_PRESSURE_HPA, pressure * 68.9476, DECIMALS_PRESSURE);
  engine.setVal(TPL_PRESSURE_BAR, pressure == 0 ? 0 : pressure / 14.5038,
                DECIMALS_PRESSURE);
  engine.setVal(TPL_PRESSURE_UNIT, myConfig.getPressureUnitAsString());

  engine.setVal(TPL_APP_VER, CFG_APPVER);
  engine.setVal(TPL_APP_BUILD, CFG_GITREV);
}

// EOF
