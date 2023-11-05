/*
MIT License

Copyright (c) 2023 Magnus

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
#include <pressureconfig.hpp>
#include <pressurepush.hpp>
#include <templating.hpp>

const char defaultFormat[] PROGMEM =
    "{"
    "\"name\" : \"${mdns}\", "
    "\"id\": \"${id}\", "
    "\"interval\": ${sleep-interval}, "
    "\"temp\": ${temp}, "
    "\"temp_units\": \"${temp-unit}\", "
    "\"pressure\": ${pressure}, "
    "\"pressure_units\": ${pressure-unit}, "
    "\"battery\": ${battery}, "
    "\"rssi\": ${rssi}, "
    "\"run-time\": ${run-time} "
    "}";

const char brewfatherFormat[] PROGMEM =
    "{"
    "\"name\" : \"${mdns}\", "
    "\"id\": \"${id}\", "
    "\"interval\": ${sleep-interval}, "
    "\"temp\": ${temp}, "
    "\"temp_units\": \"${temp-unit}\", "
    "\"pressure\": ${pressure}, "
    "\"pressure_units\": ${pressure-unit}, "
    "\"battery\": ${battery}, "
    "\"rssi\": ${rssi}, "
    "\"run-time\": ${run-time} "
    "}";

PressurePush::PressurePush(PressureConfig* config) : BasePush(config) {
  _pressConfig = config;
}

void PressurePush::push(float temp, float pressure, int runtime) {
  if (strlen(myConfig.getTargetHttpPost()) == 0) {
    Log.warning(F("Push: No target is defined, skipping push." CR));
    return;
  }

  TemplatingEngine tpl;

  tpl.setVal("${mdns}", myConfig.getMDNS());
  tpl.setVal("${id}", myConfig.getID());
  tpl.setVal("${sleep-interval}", myConfig.getPushInterval());
  tpl.setVal("${temp}", temp, DECIMALS_TEMPERATURE);
  tpl.setVal("${temp-format}", String(myConfig.getTempFormat()));
  tpl.setVal("${pressure}", pressure, DECIMALS_PRESSURE);
  tpl.setVal("${pressure-format}", myConfig.getPressureFormatTypeAsString());
  tpl.setVal("${battery}", 0, DECIMALS_BATTERY);
  tpl.setVal("${rssi}", WiFi.RSSI());
  tpl.setVal("${run-time}", runtime);

  String body;

  switch (myConfig.getPushFormatType()) {
    default:
    case PushFormatType::Default:
      body = tpl.create(defaultFormat);
      break;

    case PushFormatType::Brewfather:
      body = tpl.create(brewfatherFormat);
      break;
  }

  sendHttpPost(body);
}

// EOF
