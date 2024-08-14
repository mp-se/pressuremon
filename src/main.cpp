/*
MIT License

Copyright (c) 2022-2024 Magnus

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
#include <battery.hpp>
#include <main.hpp>
#include <pressconfig.hpp>
#include <presspush.hpp>
#include <pressuresensor.hpp>
#include <presswebhandler.hpp>
#include <serialws.hpp>
#include <utils.hpp>
#include <wificonnection.hpp>
#if CONFIG_IDF_TARGET_ESP32S2
#include <esp32s2/rom/rtc.h>
#endif
#include <esp_core_dump.h>

SerialDebug mySerial(115200L);
PressConfig myConfig(CFG_MDNSNAME, CFG_FILENAME);
WifiConnection myWifi(&myConfig, CFG_APPNAME, "password", CFG_MDNSNAME);
PressWebHandler myWebHandler(&myConfig);
SerialWebSocket mySerialWebSocket;
BatteryVoltage myBatteryVoltage;

const int loopInterval = 2000;
int loopCounter = 0;
uint32_t loopMillis = 0;
uint32_t pushMillis = 0;
uint32_t runTimeMillis = 0;
RunMode runMode = RunMode::normalMode;

void checkCoreDump();
void checkRunMode();

void setup() {
  runTimeMillis = millis();

  // delay(2000);

  // see: rtc.h for reset reasons
  Log.notice(F("Main: Reset reason %d." CR), rtc_get_reset_reason(0));
  Log.notice(F("Core dump check %d." CR), esp_core_dump_image_check());

  char cbuf[30];
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  snprintf(&cbuf[0], sizeof(cbuf), "%6x", chipId);
  Log.notice(F("Main: Started setup for %s." CR), &cbuf[0]);
  Log.notice(F("Main: Build options: %s (%s) LOGLEVEL %d " CR), CFG_APPVER,
             CFG_GITREV, LOG_LEVEL);

  checkCoreDump();
  myConfig.checkFileSystem();
  myConfig.loadFile();

  // TODO: Add option to run on battery
  // checkRunMode();

  myWifi.init();

  // No stored config, move to portal
  if (!myWifi.hasConfig() || myWifi.isDoubleResetDetected()) {
    Log.notice(
        F("Main: Missing wifi config or double reset detected, entering wifi "
          "setup." CR));
    myWifi.startAP();
    runMode = RunMode::wifiSetupMode;
  }

  myPressureSensor.setup();

  switch (runMode) {
    case RunMode::normalMode:
    case RunMode::sleepMode:
      myWifi.connect();
      // myWifi.timeSync();
      break;

    case RunMode::wifiSetupMode:
      break;
  }

  // TODO: Add option to run on battery
  /*
  if (runMode == RunMode::sleepMode) {
    Log.notice(F("Main: Running sleep mode." CR));
    myPressureSensor.loop();
    myBatteryVoltage.read();
    PressPush push(&myConfig);
    float tempC = myPressureSensor.getTemperatureC();
    float pressurePsi = myPressureSensor.getPressurePsi(true);
    push.sendAll(pressurePsi, tempC, myBatteryVoltage.getVoltage());
    LittleFS.end();
    delay(100);
    deepSleep(myConfig.getPushInterval());
  } else {*/
  myWebHandler.setupWebServer();
  mySerialWebSocket.begin(myWebHandler.getWebServer(), &EspSerial);
  mySerial.begin(&mySerialWebSocket);
  loopMillis = pushMillis = millis();
  /*}*/

  Log.notice(F("Main: Setup completed." CR));
}

void loop() {
  if (runMode != RunMode::wifiSetupMode && !myWifi.isConnected())
    myWifi.connect();

  myWebHandler.loop();
  myWifi.loop();
  mySerialWebSocket.loop();

  // Do normal housekeeping
  if (abs((int32_t)(millis() - loopMillis)) >
      loopInterval) {  // 2 seconds loop interval
    loopMillis = millis();
    loopCounter++;
    Log.notice(F("Loop: Reading sensors." CR));
    myPressureSensor.loop();
    myBatteryVoltage.read();
    float tempC = myPressureSensor.getTemperatureC();
    float pressurePsi = myPressureSensor.getPressurePsi(true);
    Log.notice(F("Loop: Reading sensors, pressure=%F psi, temp=%F C." CR), pressurePsi,
               tempC);
  }

  // Do push
  if (abs((int32_t)(millis() - pushMillis)) >
      (myConfig.getPushInterval() * 1000)) {
    pushMillis = millis();
    Log.notice(F("Loop: Pushing data to defined targets." CR));
    PressPush push(&myConfig);

    float tempC = myPressureSensor.getTemperatureC();
    float pressurePsi = myPressureSensor.getPressurePsi(true);

    // TODO: Add voltage when
    push.sendAll(pressurePsi, tempC, 0);
  }
}

void checkCoreDump() {
  esp_core_dump_summary_t *summary = static_cast<esp_core_dump_summary_t *>(
      malloc(sizeof(esp_core_dump_summary_t)));

  if (summary) {
    esp_log_level_set("esp_core_dump_elf", ESP_LOG_VERBOSE);

    if (esp_core_dump_get_summary(summary) == ESP_OK) {
      Log.notice(F("Exception cause %d." CR), summary->ex_info.exc_cause);
      Log.notice(F("PC 0x%x." CR), summary->exc_pc);

      for (int i = 0; i < summary->exc_bt_info.depth; i++) {
        Log.notice(F("PC(%d) 0x%x." CR), i, summary->exc_bt_info.bt[i]);
      }
    }
  }
}

// TODO: Add option to run on battery
/*
void checkRunMode() {
  myBatteryVoltage.read();
  float b = myBatteryVoltage.getVoltage();

  if (b < 3.0 || b > 4.2) {
    runMode = RunMode::normalMode;
  } else {
    runMode = RunMode::sleepMode;
  }

  Log.notice(F("Main: Runmode %s." CR),
             runMode == RunMode::normalMode ? "Normal" : "DeepSleep");
}*/

// EOF
