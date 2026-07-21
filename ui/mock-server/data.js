/*
 * Pressuremon - Dual License
 *
 * Open Source Use  ->  GNU General Public License v3.0 (GPL v3)
 * Commercial Use   ->  Commercial License Agreement
 *
 * Copyright (c) 2024-2026 Magnus
 *
 * For full license terms and conditions, see LICENSE and LICENSE_COMMERCIAL files.
 * Source: https://github.com/mp-se/pressuremon and https://github.com/mp-se/pressuremon-ui
 */
/*
 * Project specific data objects, should contain configData and statusData as minimum
 *
 * (c) 2023-2024 Magnus Persson
 */

export var configData = {
  // Device configuration
  id: '7376ef',
  mdns: 'pressmon3',
  temp_unit: 'C',
  // Hardware
  ota_url: 'https://www.gravitymon.com/firmware/',
  voltage_factor: 1.59,
  voltage_config: 4.15,
  battery_saving: true,
  battery_type: 0,
  sensor_type: 0,
  sensor1_type: 0,
  pressure_adjustment: 0.0,
  pressure1_adjustment: 0.0,
  temp_adjustment_value: 0.00,
  tempsensor_resolution: 9,
  custom_min_voltage: 1,
  custom_max_voltage: 3,
  custom_min_pressure: 10,
  custom_max_pressure: 20,
  custom1_min_voltage: 2,
  custom1_max_voltage: 4,
  custom1_min_pressure: 100,
  custom1_max_pressure: 200,
  // temp_adjustment: 0.0,
  // temp1_adjustment: 0.0,
  pressure_unit: "PSI",
  // Wifi
  wifi_scan_ap: true,
  wifi_portal_timeout: 120,
  wifi_connect_timeout: 20,
  wifi_ssid: 'network A',
  wifi_ssid2: '',
  wifi_pass: 'password',
  wifi_pass2: 'mypass',
  // Push - Generic
  token: 'mytoken1',
  token2: 'mytoken2',
  sleep_interval: 30,
  push_timeout: 10,
  // Push - Http Post 1
  http_post_target: 'http://post.home.arpa:9090/api/v1/ZYfjlUNeiuyu9N/telemetry',
  http_post_header1: 'Auth: Basic T7IF9DD9fF3RDddE=',
  http_post_header2: '',
  http_post_int: 1,
  // Push - Http Post 2
  http_post2_target: 'http://post2.home.arpa/ispindel',
  http_post2_header1: '',
  http_post2_header2: '',
  http_post2_int: 1,
  // Push - Http Get
  http_get_target: 'http://get.home.arpa/ispindel',
  http_get_header1: '',
  http_get_header2: '',
  http_get_int: 1,
  // Push - Influx
  influxdb2_target: 'http://influx.home.arpa:8086',
  influxdb2_org: 'myorg',
  influxdb2_bucket: 'mybucket',
  influxdb2_token: 'OijkU((jhfkh=',
  influxdb2_int: 1,
  // Push - MQTT
  mqtt_target: 'mqtt.home.arpa',
  mqtt_port: 1883,
  mqtt_user: 'user',
  mqtt_pass: 'pass',
  mqtt_int: 1,
  // Push BLE
  ble_format: 1,
  dark_mode: false
}

export var formatData = {
  http_post_format_pressure: "%7B%22name%22%3A%20%22%24%7Bmdns%7D%22%2C%20%22id%22%3A%20%22%24%7Bid%7D%22%2C%20%22token%22%3A%20%22%24%7Btoken%7D%22%2C%20%22interval%22%3A%20%24%7Bsleep%2Dinterval%7D%2C%20%22temperature%22%3A%20%24%7Btemp%7D%2C%20%22temperature%2Dunit%22%3A%20%22%24%7Btemp%2Dunit%7D%22%2C%20%22pressure%22%3A%20%24%7Bpressure%7D%2C%20%22pressure%2Dunit%22%3A%20%22%24%7Bpressure%2Dunit%7D%22%2C%20%22battery%22%3A%20%24%7Bbattery%7D%2C%20%22rssi%22%3A%20%24%7Brssi%7D%2C%20%22run%2Dtime%22%3A%20%24%7Brun%2Dtime%7D%20%7D",
  http_post2_format_pressure: "%7B%22name%22%3A%20%22%24%7Bmdns%7D%22%2C%20%22id%22%3A%20%22%24%7Bid%7D%22%2C%20%22token%22%3A%20%22%24%7Btoken%7D%22%2C%20%22interval%22%3A%20%24%7Bsleep%2Dinterval%7D%2C%20%22temperature%22%3A%20%24%7Btemp%7D%2C%20%22temperature%2Dunit%22%3A%20%22%24%7Btemp%2Dunit%7D%22%2C%20%22pressure%22%3A%20%24%7Bpressure%7D%2C%20%22pressure%2Dunit%22%3A%20%22%24%7Bpressure%2Dunit%7D%22%2C%20%22battery%22%3A%20%24%7Bbattery%7D%2C%20%22rssi%22%3A%20%24%7Brssi%7D%2C%20%22run%2Dtime%22%3A%20%24%7Brun%2Dtime%7D%20%7D",
  http_get_format_pressure: "%3Fname%3D%24%7Bmdns%7D%26id%3D%24%7Bid%7D%26token%3D%24%7Btoken2%7D%26interval%3D%24%7Bsleep%2Dinterval%7D%26temperature%3D%24%7Btemp%7D%26temperature%2Dunit%3D%24%7Btemp%2Dunit%7D%26pressure%3D%24%7Bpressure%7D%26pressure%2Dunit%3D%24%7Bpressure%2Dunit%7D%26battery%3D%24%7Bbattery%7D%26rssi%3D%24%7Brssi%7D%26run%2Dtime%3D%24%7Brun%2Dtime%7D",
  influxdb2_format_pressure: "measurement%2Chost%3D%24%7Bmdns%7D%2Cdevice%3D%24%7Bid%7D%2Ctemperature%2Dunit%3D%24%7Btemp%2Dunit%7D%2Cpressure%2Dunit%3D%24%7Bpressure%2Dunit%7D%20pressure%3D%24%7Bpressure%7D%2Ctemp%3D%24%7Btemp%7D%2Cbattery%3D%24%7Bbattery%7D%2Crssi%3D%24%7Brssi%7D%0A",
  mqtt_format_pressure: "brewcast%2Fhistory%3A%7B%22key%22%3A%20%22%24%7Bmdns%7D%22%2C%20%22data%22%3A%7B%22pressure%22%3A%20%24%7Bpressure%7D%7D%7D"
}

export var statusData = {
  id: '7376ef',
  sleep_interval: 300,
  battery: 3.81,
  pressure: 10,
  pressure1: 11,
  pressure_unit: 'PSI',
  temp: 20.2,
  // temp1: 20.5,
  temp_unit: 'C',
  sleep_mode: false,
  rssi: -56,
  app_ver: '0.5.0',
  app_build: 'gitrev',
  mdns: 'pressmon',
  board: 'D1 MINI',
  platform: 'esp32',
  wifi_ssid: 'wifi',
  total_heap: 1000,
  free_heap: 500,
  ip: '192.0.0.1',
  self_check: {
    sensor_configured: true,
    sensor_connected: true,
    temp_connected: true,
    battery_level: true,
    push_targets: true
  },
  max_sensors: 2,
  adc_found: true,
  wifi_setup: false
}

export var featureData = {
  board: 'D1_MINI',
  platform: 'esp32c3',
  app_ver: '2.0.0',
  app_build: 'gitrev',
  hardware: 'ispindel',
  firmware_file: 'firmware.bin',

  // Feature flags
  ble: true,
  no_sensors: 1,
}

// EOF
