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
#include <pressuresensor.hpp>
#include <pressurewebhandler.hpp>
#include <scheduler.hpp>
#include <utils.hpp>

// Configuration or api params
constexpr auto PARAM_APP_VER = "app-ver";
constexpr auto PARAM_APP_BUILD = "app-build";
constexpr auto PARAM_PLATFORM = "platform";
constexpr auto PARAM_STATUS = "status";

PressureWebHandler::PressureWebHandler(PressureConfig* config)
    : BaseAsyncWebHandler(config, JSON_BUFFER) {
  _config = config;
}

void PressureWebHandler::setupAsyncWebHandlers() { setupWebHandlers(); }

void PressureWebHandler::setupWebHandlers() {
  Log.notice(F("WEB : Setting up keg web handlers." CR));

  BaseAsyncWebHandler::setupAsyncWebHandlers();

  // Note! For the async implementation the order matters
  WS_BIND_URL("/api/reset", HTTP_GET, &PressureWebHandler::webReset);
  WS_BIND_URL("/api/status", HTTP_GET, &PressureWebHandler::webStatus);
  WS_BIND_URL("/api/calibrate", HTTP_GET,
              &PressureWebHandler::webCalibrateStatus);
  WS_BIND_URL("/api/calibrate", HTTP_POST, &PressureWebHandler::webCalibrate);
  WS_BIND_URL("/backup.htm", HTTP_GET, &PressureWebHandler::webBackupHtm);
}

void PressureWebHandler::webStatus(WS_PARAM) {
  Log.notice(F("WEB : webServer callback /api/status." CR));

  DynamicJsonDocument doc(1000);

  doc[PARAM_MDNS] = myConfig.getMDNS();
  doc[PARAM_ID] = myConfig.getID();
  doc[PARAM_SSID] = myConfig.getWifiSSID(0);
#if defined(ESP32)
  doc[PARAM_PLATFORM] = "esp32 async";
#elif defined(ESP32S2)
  doc[PARAM_PLATFORM] = "esp32s2 async";
#endif
  doc[PARAM_APP_VER] = CFG_APPVER;
  doc[PARAM_APP_BUILD] = CFG_GITREV;

  doc[PARAM_PRESSURE] =
      serialized(String(myPressureSensor.getPressure(true), DECIMALS_PRESSURE));
  doc[PARAM_PRESSURE_FORMAT] = myConfig.getPressureFormatTypeAsString();
  doc[PARAM_TEMP] = serialized(
      String(myPressureSensor.getTemperature(), DECIMALS_TEMPERATURE));

  doc[PARAM_TEMP_FORMAT] = String(myConfig.getTempFormat());
  doc[PARAM_BATTERY] =
      serialized(String(myBatteryVoltage.getVoltage(), DECIMALS_BATTERY));

#if LOG_LEVEL == 6
  serializeJson(doc, Serial);
  EspSerial.print(CR);
#endif

  String out;
  out.reserve(1000);
  serializeJson(doc, out);
  doc.clear();
  WS_SEND(200, "application/json", out.c_str());
}

void PressureWebHandler::webReset(WS_PARAM) {
  String id = WS_REQ_ARG(PARAM_ID);
  Log.notice(F("WEB : webServer callback /api/reset." CR));

  if (!id.compareTo(myConfig.getID())) {
    WS_SEND(200, "text/plain", "Performing reset...");
    LittleFS.end();
    delay(500);
    ESP_RESET();
  } else {
    WS_SEND(400, "text/plain", "Unknown ID.");
  }
}

void PressureWebHandler::webCalibrate(WS_PARAM) {
  String id = WS_REQ_ARG(PARAM_ID);
#if LOG_LEVEL == 6
  Log.verbose(F("WEB : webServer callback for /api/calibrate (post)." CR));
#endif
  if (!id.equalsIgnoreCase(myConfig.getID())) {
    Log.error(F("WEB : Wrong ID received %s, expected %s" CR), id.c_str(),
              myConfig.getID());
    WS_SEND(400, "text/plain", "Invalid ID.");
    return;
  }
  myScheduler.scheduleSensorCalibration();
  // myPressureSensor.calibrateSensor();
  WS_SEND(200, "text/plain", "Device calibration scheduled");
}

void PressureWebHandler::webCalibrateStatus(WS_PARAM) {
#if LOG_LEVEL == 6
  Log.verbose(F("WEB : webServer callback for /api/calibrate (get)." CR));
#endif

  DynamicJsonDocument doc(100);

  doc[PARAM_STATUS] = myScheduler.isSensorCalibrationRunning();

#if LOG_LEVEL == 6
  serializeJson(doc, Serial);
  EspSerial.print(CR);
#endif

  String out;
  out.reserve(100);
  serializeJson(doc, out);
  doc.clear();
  WS_SEND(200, "application/json", out.c_str());
}

// EOF
