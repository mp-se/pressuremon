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
#ifndef SRC_MAIN_HPP_
#define SRC_MAIN_HPP_

#include <LittleFS.h>
#include <stdlib.h>

#include <log.hpp>

constexpr auto CFG_APPNAME = "PressureMon";      // Name of firmware
constexpr auto CFG_MDNSNAME = "PressureMon";     // Network name
constexpr auto CFG_FILENAME = "/pressmon.json";  // Name of config file
constexpr auto STARTUP_FILENAME = "/startup.log";

constexpr auto JSON_BUFFER = 3000;

#if defined(ESP32)
#define ESP_RESET ESP.restart
constexpr auto PIN_LED = 2;
constexpr auto PIN_PRESSURE = D8;
constexpr auto PIN_BATTERY = A0;
#elif defined(ESP32S2)
#define ESP_RESET ESP.restart
constexpr auto PIN_LED = BUILTIN_LED;
constexpr auto PIN_PRESSURE = D8;
constexpr auto PIN_BATTERY = A0;
#else
#error "Undefined target platform"
#endif

#endif  // SRC_MAIN_HPP_
