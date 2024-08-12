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
#ifndef SRC_PRESSWEBHANDLER_HPP_
#define SRC_PRESSWEBHANDLER_HPP_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32S2) || defined(ESP32S3)
#include <WiFi.h>
#endif

#include <basewebserver.hpp>
#include <pressconfig.hpp>

class PressWebHandler : public BaseWebServer {
 protected:
  PressConfig *_config;
  volatile bool _hardwareScanTask = false;

  String _hardwareScanData;

  void setupWebHandlers();

  void webSensorCalibrate(AsyncWebServerRequest *request);
  void webHardwareScan(AsyncWebServerRequest *request);
  void webHardwareScanStatus(AsyncWebServerRequest *request);
  void webConfigGet(AsyncWebServerRequest *request);
  void webConfigPost(AsyncWebServerRequest *request, JsonVariant &json);
  void webStatus(AsyncWebServerRequest *request);
  void webHandleLogsClear(AsyncWebServerRequest *request);
  void webHandleFactoryDefaults(AsyncWebServerRequest *request);

 public:
  explicit PressWebHandler(PressConfig *config);

  void loop();
};

#endif  // SRC_PRESSWEBHANDLER_HPP_

// EOF
