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
#include <main.hpp>
#include <presspush.hpp>
#include <pressure.hpp>
#include <presswebhandler.hpp>
#include <resources.hpp>
#include <uptime.hpp>
#include <utils.hpp>

PressWebHandler::PressWebHandler(PressConfig *config)
    : BaseWebServer(config) {
  _config = config;
}

void PressWebHandler::setupWebHandlers() {
  Log.notice(F("WEB : Setting up pressure web handlers." CR));

  BaseWebServer::setupWebHandlers();
  MDNS.addService("pressmon", "tcp", 80);

  // Note! For the async implementation the order matters
  AsyncCallbackJsonWebHandler *handler;
  _server->on("/api/format", HTTP_GET,
              std::bind(&PressWebHandler::webHandleConfigFormatRead, this,
                        std::placeholders::_1));
  handler = new AsyncCallbackJsonWebHandler(
      "/api/format",
      std::bind(&PressWebHandler::webHandleConfigFormatWrite, this,
                std::placeholders::_1, std::placeholders::_2));
  _server->addHandler(handler);
  _server->on("/api/calibrate/status", HTTP_GET,
              std::bind(&PressWebHandler::webSensorCalibrateStatus, this,
                        std::placeholders::_1));
  _server->on("/api/calibrate", HTTP_GET,
              std::bind(&PressWebHandler::webSensorCalibrate, this,
                        std::placeholders::_1));
  handler = new AsyncCallbackJsonWebHandler(
      "/api/config",
      std::bind(&PressWebHandler::webConfigPost, this, std::placeholders::_1,
                std::placeholders::_2));
  _server->addHandler(handler);
  _server->on(
      "/api/config", HTTP_GET,
      std::bind(&PressWebHandler::webConfigGet, this, std::placeholders::_1));
  _server->on(
      "/api/status", HTTP_GET,
      std::bind(&PressWebHandler::webStatus, this, std::placeholders::_1));
  _server->on("/api/logs/clear", HTTP_GET,
              std::bind(&PressWebHandler::webHandleLogsClear, this,
                        std::placeholders::_1));
  _server->on("/api/hardware/status", HTTP_GET,
              std::bind(&PressWebHandler::webHardwareScanStatus, this,
                        std::placeholders::_1));
  _server->on("/api/hardware", HTTP_GET,
              std::bind(&PressWebHandler::webHardwareScan, this,
                        std::placeholders::_1));
  _server->on("/api/factory", HTTP_GET,
              std::bind(&PressWebHandler::webHandleFactoryDefaults, this,
                        std::placeholders::_1));
}

void PressWebHandler::webConfigGet(AsyncWebServerRequest *request) {
  if (!isAuthenticated(request)) {
    return;
  }

  Log.notice(F("WEB : webServer callback for /api/config(read)." CR));
  AsyncJsonResponse *response =
      new AsyncJsonResponse(false);
  JsonObject obj = response->getRoot().as<JsonObject>();
  myConfig.createJson(obj);
  response->setLength();
  request->send(response);
}

void PressWebHandler::webHandleFactoryDefaults(AsyncWebServerRequest *request) {
  if (!isAuthenticated(request)) {
    return;
  }

  Log.notice(F("WEB : webServer callback for /api/factory." CR));
  myConfig.saveFileWifiOnly();
  LittleFS.remove(ERR_FILENAME);
  LittleFS.end();
  Log.notice(F("WEB : Deleted files in filesystem, rebooting." CR));

  AsyncJsonResponse *response =
      new AsyncJsonResponse(false);
  JsonObject obj = response->getRoot().as<JsonObject>();
  obj[PARAM_SUCCESS] = true;
  obj[PARAM_MESSAGE] = "Factory reset completed, rebooting";
  response->setLength();
  request->send(response);
  delay(500);
  ESP_RESET();
}

void PressWebHandler::webConfigPost(AsyncWebServerRequest *request,
                                    JsonVariant &json) {
  if (!isAuthenticated(request)) {
    return;
  }

  Log.notice(F("WEB : webServer callback for /api/config(write)." CR));
  JsonObject obj = json.as<JsonObject>();

  /*for (JsonPair kv : obj) {
    Log.notice(F("WEB : %s=%s." CR), kv.key().c_str(),
  kv.value().as<String>().c_str());
  }*/

  myConfig.parseJson(obj);
  obj.clear();
  myConfig.saveFile();

  AsyncJsonResponse *response =
      new AsyncJsonResponse(false);
  obj = response->getRoot().as<JsonObject>();
  obj[PARAM_SUCCESS] = true;
  obj[PARAM_MESSAGE] = "Configuration updated";
  response->setLength();
  request->send(response);
}

void PressWebHandler::webHandleLogsClear(AsyncWebServerRequest *request) {
  if (!isAuthenticated(request)) {
    return;
  }

  Log.notice(F("WEB : webServer callback for /api/logs/clear." CR));
  LittleFS.remove(ERR_FILENAME);
  LittleFS.remove(ERR_FILENAME2);

  AsyncJsonResponse *response =
      new AsyncJsonResponse(false);
  JsonObject obj = response->getRoot().as<JsonObject>();
  obj[PARAM_SUCCESS] = true;
  obj[PARAM_MESSAGE] = "Logfiles removed";
  response->setLength();
  request->send(response);
}

void PressWebHandler::webSensorCalibrate(AsyncWebServerRequest *request) {
  if (!isAuthenticated(request)) {
    return;
  }

  Log.notice(F("WEB : webServer callback /api/calibrate." CR));

  _calibrateTask = true;

  AsyncJsonResponse *response =
      new AsyncJsonResponse(false);
  JsonObject obj2 = response->getRoot().as<JsonObject>();
  obj2[PARAM_SUCCESS] = true;
  obj2[PARAM_MESSAGE] = "Sensor calibration completed";
  response->setLength();
  request->send(response);
}

void PressWebHandler::webSensorCalibrateStatus(AsyncWebServerRequest *request) {
  if (!isAuthenticated(request)) {
    return;
  }

  Log.notice(F("WEB : webServer callback for /api/calibrate/status." CR));

  if (_calibrateTask) {
    AsyncJsonResponse *response =
        new AsyncJsonResponse(false);
    JsonObject obj = response->getRoot().as<JsonObject>();
    obj[PARAM_STATUS] = static_cast<bool>(_calibrateTask);
    obj[PARAM_SUCCESS] = false;
    obj[PARAM_MESSAGE] =
        _calibrateTask ? "Calibration running" : "No calibration running";
    response->setLength();
    request->send(response);
  } else {
    request->send(200, "application/json");
  }
}

void PressWebHandler::webStatus(AsyncWebServerRequest *request) {
  Log.notice(F("WEB : webServer callback /api/status." CR));

  AsyncJsonResponse *response =
      new AsyncJsonResponse(false);
  JsonObject obj = response->getRoot().as<JsonObject>();

  obj[PARAM_PRESSURE] =
      serialized(String(myPressureSensor.getPressure(), DECIMALS_PRESSURE));
  obj[PARAM_PRESSURE_UNIT] = String(myConfig.getPressureUnit());

  obj[PARAM_MDNS] = myConfig.getMDNS();
  obj[PARAM_ID] = myConfig.getID();
  obj[PARAM_RSSI] = WiFi.RSSI();
  obj[PARAM_SSID] = myConfig.getWifiSSID(0);
#if defined(ESP32S2)
  obj[PARAM_PLATFORM] = "esp32s2";
#else
#error "Undefined target"
#endif
  obj[PARAM_APP_VER] = CFG_APPVER;
  obj[PARAM_APP_BUILD] = CFG_GITREV;

  obj[PARAM_TEMP] =
      serialized(String(myPressureSensor.getTemperature(), DECIMALS_TEMP));
  obj[PARAM_TEMP_FORMAT] = String(myConfig.getTempFormat());

  obj[PARAM_UPTIME_SECONDS] = myUptime.getSeconds();
  obj[PARAM_UPTIME_MINUTES] = myUptime.getMinutes();
  obj[PARAM_UPTIME_HOURS] = myUptime.getHours();
  obj[PARAM_UPTIME_DAYS] = myUptime.getDays();

  // TODO: Add battery

  obj[PARAM_TOTAL_HEAP] = ESP.getHeapSize();
  obj[PARAM_FREE_HEAP] = ESP.getFreeHeap();
  obj[PARAM_IP] = WiFi.localIP().toString();
  obj[PARAM_WIFI_SETUP] = (runMode == RunMode::wifiSetupMode) ? true : false;
  response->setLength();
  request->send(response);
}

void PressWebHandler::webHardwareScan(AsyncWebServerRequest *request) {
  if (!isAuthenticated(request)) {
    return;
  }

  Log.notice(F("WEB : webServer callback for /api/hardware." CR));
  _hardwareScanTask = true;
  _hardwareScanData = "";
  AsyncJsonResponse *response =
      new AsyncJsonResponse(false);
  JsonObject obj = response->getRoot().as<JsonObject>();
  obj[PARAM_SUCCESS] = true;
  obj[PARAM_MESSAGE] = "Scheduled hardware scanning";
  response->setLength();
  request->send(response);
}

void PressWebHandler::webHardwareScanStatus(AsyncWebServerRequest *request) {
  if (!isAuthenticated(request)) {
    return;
  }

  Log.notice(F("WEB : webServer callback for /api/hardware/status." CR));

  if (_hardwareScanTask || !_hardwareScanData.length()) {
    AsyncJsonResponse *response =
        new AsyncJsonResponse(false);
    JsonObject obj = response->getRoot().as<JsonObject>();
    obj[PARAM_STATUS] = static_cast<bool>(_hardwareScanTask);
    obj[PARAM_SUCCESS] = false;
    obj[PARAM_MESSAGE] =
        _hardwareScanTask ? "Hardware scanning running" : "No scanning running";
    response->setLength();
    request->send(response);
  } else {
    request->send(200, "application/json", _hardwareScanData);
  }
}

String PressWebHandler::readFile(String fname) {
  File file = LittleFS.open(fname, "r");
  if (file) {
    char buf[file.size() + 1];
    memset(&buf[0], 0, file.size() + 1);
    file.readBytes(&buf[0], file.size());
    file.close();
    Log.notice(F("WEB : Read template data from %s." CR), fname.c_str());
    return String(&buf[0]);
  }
  return "";
}

bool PressWebHandler::writeFile(String fname, String data) {
  if (data.length()) {
    data = urldecode(data);
    File file = LittleFS.open(fname, "w");
    if (file) {
      Log.notice(F("WEB : Storing template data in %s." CR), fname.c_str());
      file.write((unsigned char *)data.c_str(), data.length());
      file.close();
      return true;
    }
  } else {
    Log.notice(
        F("WEB : No template data to store in %s, reverting to default." CR),
        fname.c_str());
    LittleFS.remove(fname);
    return true;
  }

  return false;
}

void PressWebHandler::webHandleConfigFormatWrite(AsyncWebServerRequest *request,
                                                 JsonVariant &json) {
  if (!isAuthenticated(request)) {
    return;
  }

  Log.notice(F("WEB : webServer callback for /api/config/format(post)." CR));

  JsonObject obj = json.as<JsonObject>();
  int success = 0;

  if (!obj[PARAM_FORMAT_POST].isNull()) {
    success += writeFile(TPL_FNAME_POST, obj[PARAM_FORMAT_POST]) ? 1 : 0;
  }
  if (!obj[PARAM_FORMAT_POST2].isNull()) {
    success += writeFile(TPL_FNAME_POST2, obj[PARAM_FORMAT_POST2]) ? 1 : 0;
  }
  if (!obj[PARAM_FORMAT_GET].isNull()) {
    success += writeFile(TPL_FNAME_GET, obj[PARAM_FORMAT_GET]) ? 1 : 0;
  }
  if (!obj[PARAM_FORMAT_INFLUXDB].isNull()) {
    success +=
        writeFile(TPL_FNAME_INFLUXDB, obj[PARAM_FORMAT_INFLUXDB]) ? 1 : 0;
  }
  if (!obj[PARAM_FORMAT_MQTT].isNull()) {
    success += writeFile(TPL_FNAME_MQTT, obj[PARAM_FORMAT_MQTT]) ? 1 : 0;
  }

  AsyncJsonResponse *response =
      new AsyncJsonResponse(false);
  obj = response->getRoot().as<JsonObject>();
  obj[PARAM_SUCCESS] = success > 0 ? true : false;
  obj[PARAM_MESSAGE] = success > 0 ? "Format template stored"
                                   : "Failed to store format template";
  response->setLength();
  request->send(response);
}

void PressWebHandler::webHandleConfigFormatRead(
    AsyncWebServerRequest *request) {
  if (!isAuthenticated(request)) {
    return;
  }
  Log.notice(F("WEB : webServer callback for /api/config/format(read)." CR));

  AsyncJsonResponse *response =
      new AsyncJsonResponse(false);
  JsonObject obj = response->getRoot().as<JsonObject>();
  String s;

  s = readFile(TPL_FNAME_POST);
  obj[PARAM_FORMAT_POST] =
      s.length() ? urlencode(s) : urlencode(String(&httpPostFormat[0]));
  s = readFile(TPL_FNAME_POST2);
  obj[PARAM_FORMAT_POST2] =
      s.length() ? urlencode(s) : urlencode(String(&httpPostFormat[0]));
  s = readFile(TPL_FNAME_GET);
  obj[PARAM_FORMAT_GET] =
      s.length() ? urlencode(s) : urlencode(String(&httpGetFormat[0]));
  s = readFile(TPL_FNAME_INFLUXDB);
  obj[PARAM_FORMAT_INFLUXDB] =
      s.length() ? urlencode(s) : urlencode(String(&influxDbFormat[0]));
  s = readFile(TPL_FNAME_MQTT);
  obj[PARAM_FORMAT_MQTT] =
      s.length() ? urlencode(s) : urlencode(String(&mqttFormat[0]));

  response->setLength();
  request->send(response);
}

void PressWebHandler::loop() {
  BaseWebServer::loop();

  if (_calibrateTask) {
    myPressureSensor.calibrateSensor();
    _calibrateTask = false;
  }

  if (_hardwareScanTask) {
    JsonDocument doc;
    JsonObject obj = doc.as<JsonObject>();
    obj[PARAM_STATUS] = false;
    obj[PARAM_SUCCESS] = true;
    obj[PARAM_MESSAGE] = "";
    Log.notice(F("WEB : Scanning hardware." CR));

    // Scan the i2c bus for devices
    JsonArray i2c = obj.createNestedArray(PARAM_I2C);

    // Scan bus #1
    for (int i = 1; i < 127; i++) {
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      Wire.beginTransmission(i);
      int err = Wire.endTransmission();

      if (err == 0) {
        JsonObject sensor = i2c.createNestedObject();
        sensor[PARAM_ADRESS] = "0x" + String(i, 16);
      }
    }

    // Scan onewire
    /*
    JsonArray onew = obj.createNestedArray(PARAM_ONEWIRE);

    for (int i = 0; i < mySensors.getDS18Count(); i++) {
      DeviceAddress adr;
      JsonObject sensor = onew.createNestedObject();
      mySensors.getAddress(&adr[0], i);
      sensor[PARAM_ADRESS] = String(adr[0], 16) + String(adr[1], 16) +
                             String(adr[2], 16) + String(adr[3], 16) +
                             String(adr[4], 16) + String(adr[5], 16) +
                             String(adr[6], 16) + String(adr[7], 16);
      switch (adr[0]) {
        case DS18S20MODEL:
          sensor[PARAM_FAMILY] = "DS18S20";
          break;
        case DS18B20MODEL:
          sensor[PARAM_FAMILY] = "DS18B20";
          break;
        case DS1822MODEL:
          sensor[PARAM_FAMILY] = "DS1822";
          break;
        case DS1825MODEL:
          sensor[PARAM_FAMILY] = "DS1825";
          break;
        case DS28EA00MODEL:
          sensor[PARAM_FAMILY] = "DS28EA00";
          break;
      }
      sensor[PARAM_RESOLUTION] = mySensors.getResolution();
    }*/

    JsonObject cpu = obj.createNestedObject(PARAM_CHIP);

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    cpu[PARAM_REVISION] = chip_info.revision;
    cpu[PARAM_CORES] = chip_info.cores;

    JsonArray feature = cpu.createNestedArray(PARAM_FEATURES);

    if (chip_info.features & CHIP_FEATURE_EMB_FLASH)
      feature.add("embedded flash");
    if (chip_info.features & CHIP_FEATURE_WIFI_BGN)
      feature.add("Embedded Flash");
    if (chip_info.features & CHIP_FEATURE_EMB_FLASH) feature.add("2.4Ghz WIFI");
    if (chip_info.features & CHIP_FEATURE_BLE) feature.add("Bluetooth LE");
    if (chip_info.features & CHIP_FEATURE_BT) feature.add("Bluetooth Classic");
    if (chip_info.features & CHIP_FEATURE_IEEE802154)
      feature.add("IEEE 802.15.4/LR-WPAN");
    if (chip_info.features & CHIP_FEATURE_EMB_PSRAM)
      feature.add("Embedded PSRAM");

    switch (chip_info.model) {
      case CHIP_ESP32:
        cpu[PARAM_FAMILY] = "ESP32";
        break;
      case CHIP_ESP32S2:
        cpu[PARAM_FAMILY] = "ESP32S2";
        break;
      case CHIP_ESP32S3:
        cpu[PARAM_FAMILY] = "ESP32S3";
        break;
      case CHIP_ESP32C3:
        cpu[PARAM_FAMILY] = "ESP32C3";
        break;
      case CHIP_ESP32H2:
        cpu[PARAM_FAMILY] = "ESP32H2";
        break;
      default:
        cpu[PARAM_FAMILY] = String(chip_info.model);
        break;
    }

    serializeJson(obj, _hardwareScanData);
    Log.notice(F("WEB : Scan complete %s." CR), _hardwareScanData.c_str());
    _hardwareScanTask = false;
  }
}

// EOF
