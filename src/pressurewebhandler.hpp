/*
MIT License

Copyright (c) 2021-22 Magnus

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
#ifndef SRC_PRESSUREWEBHANDLER_HPP_
#define SRC_PRESSUREWEBHANDLER_HPP_

#include <WebServer.h>
#include <WiFi.h>

#include <baseasyncwebhandler.hpp>
#include <pressureconfig.hpp>

extern const uint8_t backupHtmStart[] asm("_binary_html_backup_min_htm_start");
extern const uint8_t backupHtmEnd[] asm("_binary_html_backup_min_htm_end");
extern const uint8_t brewpiHtmStart[] asm("_binary_html_brewpi_min_htm_start");
extern const uint8_t brewpiHtmEnd[] asm("_binary_html_brewpi_min_htm_end");

#define WS_BIND_URL(url, http, func) \
  _server->on(url, http, std::bind(func, this, std::placeholders::_1))
#define WS_PARAM AsyncWebServerRequest* request
#define WS_SEND_STATIC(ptr, size) \
  request->send_P(200, "text/html", (const uint8_t*)ptr, size)
#define WS_REQ_ARG(key) request->arg(key)
#define WS_REQ_ARG_NAME(idx) request->argName(idx)
#define WS_REQ_ARG_CNT() request->args()
#define WS_REQ_HAS_ARG(key) request->hasArg(key)
#define WS_SEND(code, type, text)                                              \
  AsyncWebServerResponse* response = request->beginResponse(code, type, text); \
  response->addHeader("Access-Control-Allow-Origin", "*");                     \
  request->send(response);

class PressureWebHandler : public BaseAsyncWebHandler {
 protected:
  PressureConfig* _config;

  void setupWebHandlers();
  void setupAsyncWebHandlers();
  void webConfigGet(WS_PARAM);
  void webConfigPost(WS_PARAM);
  void webStatus(WS_PARAM);
  void webReset(WS_PARAM);
  void webCalibrate(WS_PARAM);
  void webCalibrateStatus(WS_PARAM);
  void webBackupHtm(WS_PARAM) {
    WS_SEND_STATIC((const char*)backupHtmStart,
                   strlen(reinterpret_cast<const char*>(&backupHtmStart[0])));
  }

 public:
  explicit PressureWebHandler(PressureConfig* config);
};

#endif  // SRC_PRESSUREWEBHANDLER_HPP_

// EOF
