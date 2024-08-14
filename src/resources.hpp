/*
MIT License

Copyright (c) 2024 Magnus

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
#ifndef SRC_RESOURCES_HPP_
#define SRC_RESOURCES_HPP_

constexpr auto PARAM_PRESSURE_UNIT = "pressure_unit";
constexpr auto PARAM_PRESSURE_ZERO_CORRECTION = "pressure_zero_correction";
constexpr auto PARAM_PRESSURE_SENSOR_TYPE = "pressure_sensor_type";
constexpr auto PARAM_VOLTAGE_FACTOR = "voltage_factor";

constexpr auto PRESSURE_HPA = "hpa";
constexpr auto PRESSURE_BAR = "bar";
constexpr auto PRESSURE_PSI = "psi";

// Configuration or api params
constexpr auto PARAM_APP_VER = "app_ver";
constexpr auto PARAM_APP_BUILD = "app_build";
constexpr auto PARAM_PLATFORM = "platform";
constexpr auto PARAM_PRESSURE = "pressure";
constexpr auto PARAM_TEMP = "temp";

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

constexpr auto PARAM_FORMAT_POST = "http_post_format";
constexpr auto PARAM_FORMAT_POST2 = "http_post2_format";
constexpr auto PARAM_FORMAT_GET = "http_get_format";
constexpr auto PARAM_FORMAT_INFLUXDB = "influxdb2_format";
constexpr auto PARAM_FORMAT_MQTT = "mqtt_format";

#endif  // SRC_RESOURCES_HPP_

// EOF
