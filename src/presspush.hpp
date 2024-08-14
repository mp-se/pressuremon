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
#ifndef SRC_PRESSPUSH_HPP_
#define SRC_PRESSPUSH_HPP_

#include <basepush.hpp>
#include <pressconfig.hpp>
#include <templating.hpp>

constexpr auto TPL_MDNS = "${mdns}";
constexpr auto TPL_ID = "${id}";
constexpr auto TPL_SLEEP_INTERVAL = "${sleep-interval}";
constexpr auto TPL_TEMP = "${temp}";
constexpr auto TPL_TEMP_C = "${temp-c}";
constexpr auto TPL_TEMP_F = "${temp-f}";
constexpr auto TPL_TEMP_UNITS = "${temp-unit}";  // C or F
constexpr auto TPL_BATTERY = "${battery}";
constexpr auto TPL_BATTERY_PERCENT = "${battery-percent}";
constexpr auto TPL_RSSI = "${rssi}";
constexpr auto TPL_PRESSURE = "${pressure}";
constexpr auto TPL_PRESSURE_FORMAT = "${pressure-format}";
constexpr auto TPL_PRESSURE_PSI = "${pressure-psi}";
constexpr auto TPL_PRESSURE_BAR = "${pressure-bar}";
constexpr auto TPL_PRESSURE_HPA = "${pressure-hpa}";
constexpr auto TPL_APP_VER = "${app-ver}";
constexpr auto TPL_APP_BUILD = "${app-build}";

constexpr auto TPL_FNAME_POST = "/http-1.tpl";
constexpr auto TPL_FNAME_POST2 = "/http-2.tpl";
constexpr auto TPL_FNAME_GET = "/http-3.tpl";
constexpr auto TPL_FNAME_INFLUXDB = "/influxdb.tpl";
constexpr auto TPL_FNAME_MQTT = "/mqtt.tpl";

extern const char httpPostFormat[] PROGMEM;
extern const char httpGetFormat[] PROGMEM;
extern const char influxDbFormat[] PROGMEM;
extern const char mqttFormat[] PROGMEM;

class PressPush : public BasePush {
 private:
  PressConfig* _pressConfig;
  String _baseTemplate;

 public:
  explicit PressPush(PressConfig* pressConfig);

  enum Templates {
    TEMPLATE_HTTP1 = 0,
    TEMPLATE_HTTP2 = 1,
    TEMPLATE_HTTP3 = 2,
    TEMPLATE_INFLUX = 3,
    TEMPLATE_MQTT = 4
  };

  void sendAll(float pressure, float tempC, float voltage);

  const char* getTemplate(Templates t, bool useDefaultTemplate = false);
  void clearTemplate() { _baseTemplate.clear(); }
  void setupTemplateEngine(TemplatingEngine& engine, float pressure,
                           float tempC, float voltage);
  int getLastCode() { return _lastResponseCode; }
  bool getLastSuccess() { return _lastSuccess; }
};

#endif  // SRC_PRESSPUSH_HPP_

// EOF
