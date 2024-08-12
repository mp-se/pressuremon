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
#include <presspush.hpp>
#include <presswebhandler.hpp>
#include <main.hpp>
#include <pressuresensor.hpp>
#include <uptime.hpp>
#include <utils.hpp>

// Configuration or api params
constexpr auto PARAM_APP_VER = "app_ver";
constexpr auto PARAM_APP_BUILD = "app_build";
constexpr auto PARAM_PLATFORM = "platform";
constexpr auto PARAM_PRESSURE = "pressure";

// Other values
constexpr auto PARAM_TOTAL_HEAP = "total_heap";
constexpr auto PARAM_FREE_HEAP = "free_heap";
constexpr auto PARAM_IP = "ip";
constexpr auto PARAM_I2C = "i2c";
// constexpr auto PARAM_ONEWIRE = "onewire";
// constexpr auto PARAM_RESOLUTION = "resolution";
constexpr auto PARAM_ADRESS = "adress";
constexpr auto PARAM_FAMILY = "family";
constexpr auto PARAM_CHIP = "chip";
constexpr auto PARAM_REVISION = "revision";
constexpr auto PARAM_CORES = "cores";
constexpr auto PARAM_FEATURES = "features";
constexpr auto PARAM_WIFI_SETUP = "wifi_setup";
constexpr auto PARAM_ONEWIRE = "onewire";
constexpr auto PARAM_RESOLUTION = "resolution";
constexpr auto PARAM_UPTIME_SECONDS = "uptime_seconds";
constexpr auto PARAM_UPTIME_MINUTES = "uptime_minutes";
constexpr auto PARAM_UPTIME_HOURS = "uptime_hours";
constexpr auto PARAM_UPTIME_DAYS = "uptime_days";

PressWebHandler::PressWebHandler(PressConfig *config)
    : BaseWebServer(config, JSON_BUFFER) {
  _config = config;
}

void PressWebHandler::setupWebHandlers() {
  Log.notice(F("WEB : Setting up pressure web handlers." CR));

  BaseWebServer::setupWebHandlers();

  MDNS.addService("pressmon", "tcp", 80);

  // Note! For the async implementation the order matters

  AsyncCallbackJsonWebHandler *handler;
  _server->on(
      "/api/calibrate", HTTP_GET,
      std::bind(&PressWebHandler::webSensorCalibrate, this, std::placeholders::_1));
  handler = new AsyncCallbackJsonWebHandler(
      "/api/config",
      std::bind(&PressWebHandler::webConfigPost, this, std::placeholders::_1,
                std::placeholders::_2),
      JSON_BUFFER_SIZE_L);
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
  _server->on(
      "/api/hardware", HTTP_GET,
      std::bind(&PressWebHandler::webHardwareScan, this, std::placeholders::_1));
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
      new AsyncJsonResponse(false, JSON_BUFFER_SIZE_XL);
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
      new AsyncJsonResponse(false, JSON_BUFFER_SIZE_S);
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
      new AsyncJsonResponse(false, JSON_BUFFER_SIZE_S);
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
      new AsyncJsonResponse(false, JSON_BUFFER_SIZE_S);
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

  // TODO: Calibrate sensor

  AsyncJsonResponse *response =
      new AsyncJsonResponse(false, JSON_BUFFER_SIZE_S);
  JsonObject obj2 = response->getRoot().as<JsonObject>();
  obj2[PARAM_SUCCESS] = true;
  obj2[PARAM_MESSAGE] = "Sensor calibration completed";
  response->setLength();
  request->send(response);
}

void PressWebHandler::webStatus(AsyncWebServerRequest *request) {
  Log.notice(F("WEB : webServer callback /api/status." CR));

  AsyncJsonResponse *response =
      new AsyncJsonResponse(false, JSON_BUFFER_SIZE_L);
  JsonObject obj = response->getRoot().as<JsonObject>();

  // TODO: Add the pressure sensor values

  obj[PARAM_MDNS] = myConfig.getMDNS();
  obj[PARAM_ID] = myConfig.getID();
  obj[PARAM_RSSI] = WiFi.RSSI();
  obj[PARAM_SSID] = myConfig.getWifiSSID(0);
#if defined(ESP8266)
  obj[PARAM_PLATFORM] = "esp8266";
#elif defined(ESP32S2)
  obj[PARAM_PLATFORM] = "esp32s2";
#elif defined(ESP32S3)
  obj[PARAM_PLATFORM] = "esp32s3";
#else
#error "Undefined target"
#endif
  obj[PARAM_APP_VER] = CFG_APPVER;
  obj[PARAM_APP_BUILD] = CFG_GITREV;
  
  // TODO: Add pressure unit
  // obj[PARAM_WEIGHT_UNIT] = myConfig.getWeightUnit();
  obj[PARAM_TEMP_FORMAT] = String(myConfig.getTempFormat());

  obj[PARAM_UPTIME_SECONDS] = myUptime.getSeconds();
  obj[PARAM_UPTIME_MINUTES] = myUptime.getMinutes();
  obj[PARAM_UPTIME_HOURS] = myUptime.getHours();
  obj[PARAM_UPTIME_DAYS] = myUptime.getDays();

  /*float f = myTemp.getLastTempC();

  if (!isnan(f)) {
    obj[PARAM_TEMP] = serialized(String(convertOutgoingTemperature(f), 2));
  }

  float h = myTemp.getLastHumidity();

  if (!isnan(h)) {
    obj[PARAM_HUMIDITY] = serialized(String(h, 2));
  }

  float p = myTemp.getLastPressure();

  if (!isnan(p)) {
    obj[PARAM_PRESSURE] = serialized(String(p, 2));
  }*/

#if defined(ESP8266)
  obj[PARAM_TOTAL_HEAP] = 81920;
  obj[PARAM_FREE_HEAP] = ESP.getFreeHeap();
  obj[PARAM_IP] = WiFi.localIP().toString();
#else
  obj[PARAM_TOTAL_HEAP] = ESP.getHeapSize();
  obj[PARAM_FREE_HEAP] = ESP.getFreeHeap();
  obj[PARAM_IP] = WiFi.localIP().toString();
#endif
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
      new AsyncJsonResponse(false, JSON_BUFFER_SIZE_S);
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
        new AsyncJsonResponse(false, JSON_BUFFER_SIZE_L);
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

void PressWebHandler::loop() {
#if defined(ESP8266)
  MDNS.update();
#endif
  BaseWebServer::loop();

  if (_hardwareScanTask) {
    DynamicJsonDocument doc(JSON_BUFFER_SIZE_L);
    JsonObject obj = doc.createNestedObject();
    obj[PARAM_STATUS] = false;
    obj[PARAM_SUCCESS] = true;
    obj[PARAM_MESSAGE] = "";
    Log.notice(F("WEB : Scanning hardware." CR));

    // Scan the i2c bus for devices
    /*
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
    */

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

#if defined(ESP8266)
    cpu[PARAM_FAMILY] = "ESP8266";
#else
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
#endif

    serializeJson(obj, _hardwareScanData);
    Log.notice(F("WEB : Scan complete %s." CR), _hardwareScanData.c_str());
    _hardwareScanTask = false;
  }
}

// EOF
