import { createPinia, setActivePinia } from 'pinia'
import { vi } from 'vitest'

export function createTestingPinia() {
  const pinia = createPinia()
  setActivePinia(pinia)
  return pinia
}

export function resetAllMocks() {
  vi.clearAllMocks()
}

export const mockConfigState = {
  id: 'test-id',
  mdns: 'pressuremon-test',
  temp_unit: 'C',
  ota_url: '',
  voltage_factor: 0,
  voltage_config: 0,
  gyro_temp: false,
  battery_saving: false,
  battery_type: 0,
  tempsensor_resolution: 0,
  temp_adjustment_value: 0,
  voltage_pin: 0,
  sensor_type: 0,
  sensor1_type: 0,
  pressure_adjustment: 0,
  pressure1_adjustment: 0,
  custom_min_voltage: 0,
  custom_max_voltage: 0,
  custom_min_pressure: 0,
  custom_max_pressure: 0,
  custom1_min_voltage: 0,
  custom1_max_voltage: 0,
  custom1_min_pressure: 0,
  custom1_max_pressure: 0,
  pressure_unit: 'psi',
  wifi_portal_timeout: 0,
  wifi_connect_timeout: 0,
  wifi_ssid: 'test-ssid',
  wifi_ssid2: '',
  wifi_pass: 'test-pass',
  wifi_pass2: '',
  wifi_direct_ssid: '',
  wifi_direct_pass: '',
  use_wifi_direct: false,
  wifi_scan_ap: false,
  token: 'token123',
  token2: '',
  sleep_interval: 60,
  push_timeout: 5000,
  http_post_target: '',
  http_post_header1: '',
  http_post_header2: '',
  http_post_int: 0,
  http_post_format_pressure: '',
  http_post2_target: '',
  http_post2_header1: '',
  http_post2_header2: '',
  http_post2_int: 0,
  http_post2_format_pressure: '',
  http_get_target: '',
  http_get_header1: '',
  http_get_header2: '',
  http_get_int: 0,
  http_get_format_pressure: '',
  influxdb2_target: '',
  influxdb2_org: '',
  influxdb2_bucket: '',
  influxdb2_token: '',
  influxdb2_int: 0,
  influxdb2_format_pressure: '',
  mqtt_target: '',
  mqtt_port: '',
  mqtt_user: '',
  mqtt_pass: '',
  mqtt_int: 0,
  mqtt_format_pressure: '',
  ble_format: 0,
  dark_mode: false
}

export const mockStatusState = {
  id: 'test-id',
  pressure: 10.5,
  pressure1: 11.1,
  pressure_unit: 'PSI',
  temp: 20,
  temp1: 20.5,
  temp_unit: 'C',
  sleep_interval: 60,
  battery: 3.7,
  sleep_mode: false,
  rssi: -50,
  mdns: 'pressuremon-test',
  wifi_ssid: 'test-ssid',
  ip: '192.168.1.100',
  total_heap: 64,
  free_heap: 32,
  self_check: {
    sensor_configured: true,
    sensor_connected: true,
    temp_connected: true,
    battery_level: true,
    push_targets: true
  },
  wifi_setup: true,
  max_sensors: 1,
  adc_found: false,
  connected: true
}

export const mockGlobalState = {
  id: 'test-id',
  platform: 'ESP32',
  board: 'ESP32-DevKit',
  app_ver: '0.7.0',
  app_build: '42',
  firmware_file: 'pressuremon.bin',
  ui: {
    enableVoltageFragment: true,
    enableManualWifiEntry: false,
    enableScanForStrongestAp: true
  },
  feature: {
    ble: true,
    no_sensors: 1
  },
  initialized: true,
  disabled: false,
  configChanged: false,
  messageError: '',
  messageWarning: '',
  messageSuccess: '',
  messageInfo: ''
}
