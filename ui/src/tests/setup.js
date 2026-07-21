import { vi } from 'vitest'
import { config as vtConfig } from '@vue/test-utils'

class LocalStorageMock {
  constructor() {
    this.store = {}
  }

  clear() {
    this.store = {}
  }

  getItem(key) {
    return this.store[key] || null
  }

  setItem(key, value) {
    this.store[key] = String(value)
  }

  removeItem(key) {
    delete this.store[key]
  }

  get length() {
    return Object.keys(this.store).length
  }

  key(index) {
    const keys = Object.keys(this.store)
    return keys[index] || null
  }
}

Object.defineProperty(window, 'localStorage', {
  value: new LocalStorageMock()
})

vi.mock('@/modules/configStore', () => ({
  useConfigStore: vi.fn(() => ({
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
    wifi_ssid: '',
    wifi_ssid2: '',
    wifi_pass: '',
    wifi_pass2: '',
    wifi_direct_ssid: '',
    wifi_direct_pass: '',
    use_wifi_direct: false,
    wifi_scan_ap: false,
    token: '',
    token2: '',
    sleep_interval: 600,
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
    influxdb2_format_gravity: '',
    mqtt_target: '',
    mqtt_port: '',
    mqtt_user: '',
    mqtt_pass: '',
    mqtt_int: 0,
    mqtt_format_pressure: '',
    mqtt_format_gravity: '',
    http_post_format_gravity: '',
    http_post2_format_gravity: '',
    http_get_format_gravity: '',
    gravity_unit: 'SG',
    ble_format: 0,
    dark_mode: false,
    internal_temp_unit: 'C',
    formula_calibration_temp: 20,
    toJson: vi.fn(),
    convertTemp: vi.fn(),
    convertTempToC: vi.fn(),
    convertTempToF: vi.fn(),
    setSleepMode: vi.fn(async () => true),
    load: vi.fn(async () => true),
    loadFormat: vi.fn(async () => true),
    sendConfig: vi.fn(async () => true),
    sendFormat: vi.fn(async () => true),
    saveAll: vi.fn(async () => true),
    restart: vi.fn(async () => true),
    runWifiScan: vi.fn(async () => ({ success: true, data: { networks: [] } })),
    runHardwareScan: vi.fn(async () => ''),
    sendFilesystemRequest: vi.fn((data, callback) => {
      if (data.command === 'get') callback(true, '')
      else callback(true)
    }),
    runPushTest: vi.fn(async () => true),
    getPushTestStatus: vi.fn(async () => ({
      success: true,
      data: { status: false, success: true }
    })),
    $state: {},
    $subscribe: vi.fn()
  }))
}))

vi.mock('@/modules/globalStore', () => ({
  useGlobalStore: vi.fn(() => ({
    id: 'test-id',
    initialized: true,
    disabled: false,
    configChanged: false,
    messageError: '',
    messageWarning: '',
    messageSuccess: '',
    messageInfo: '',
    platform: 'ESP32',
    board: 'ESP32-DevKit',
    app_ver: '0.7.0',
    app_build: '42',
    firmware_file: 'pressuremon.bin',
    ui: {
      enableVoltageFragment: true,
      enableManualWifiEntry: false,
      enableScanForStrongestAp: true,
      enableGravity: true,
      enablePressure: true
    },
    feature: {
      ble: true,
      no_sensors: 1,
      filter: true,
      velocity: true,
      charging: true
    },
    clearMessages: vi.fn(),
    load: vi.fn(async () => true),
    uiVersion: '0.7.0-test',
    uiBuild: '1',
    isError: false,
    isWarning: false,
    isSuccess: false,
    isInfo: false,
    $state: {}
  }))
}))

vi.mock('@/modules/statusStore', () => ({
  useStatusStore: vi.fn(() => ({
    id: 'test-id',
    pressure: 10.5,
    pressure1: 11.1,
    pressure_unit: 'PSI',
    temp: 20,
    temp1: 20.5,
    temp_unit: 'C',
    battery: 3.7,
    rssi: -50,
    sleep_mode: false,
    ispindel_config: false,
    wifi_setup: false,
    connected: true,
    self_check: {
      sensor_configured: true,
      sensor_connected: true,
      temp_connected: true,
      battery_level: true,
      push_targets: true
    },
    load: vi.fn(async () => true),
    $state: {}
  }))
}))

vi.mock('@mp-se/espframework-ui-components', () => {
  const blockStub = { template: '<div><slot /></div>' }
  const inlineStub = { template: '<span />' }

  return {
    logError: vi.fn(),
    logDebug: vi.fn(),
    logInfo: vi.fn(),
    version: 'test-1.0.0',
    tempToF: (c) => (c * 9) / 5 + 32,
    tempToC: (f) => ((f - 32) * 5) / 9,
    psiToBar: (psi) => psi * 0.0689476,
    psiToKPa: (psi) => psi * 6.89476,
    barToPsi: (bar) => bar / 0.0689476,
    kpaToPsi: (kpa) => kpa / 6.89476,
    roundVal: (value, precision) => parseFloat(Number(value).toFixed(precision)),
    validateCurrentForm: vi.fn(() => true),
    isValidJson: vi.fn(() => true),
    isValidFormData: vi.fn(() => true),
    isValidMqttData: vi.fn(() => true),
    useTimers: () => ({
      createInterval: vi.fn(() => ({ start: vi.fn(), stop: vi.fn() })),
      clearManagedInterval: vi.fn(),
      createTimeout: vi.fn((cb) => setTimeout(cb, 0))
    }),
    useFetch: () => ({
      managedFetch: vi.fn(async () => ({}))
    }),
    sharedHttpClient: {
      auth: vi.fn(async () => true),
      ping: vi.fn(async () => true),
      getJson: vi.fn(async () => ({})),
      postJson: vi.fn(async () => ({})),
      putJson: vi.fn(async () => ({})),
      deleteJson: vi.fn(async () => ({})),
      request: vi.fn(async () => ({
        json: async () => ({})
      })),
      filesystemRequest: vi.fn(async () => ({ success: false })),
      uploadFile: vi.fn(async () => ({ success: true })),
      createWebSocket: vi.fn((path, handlers = {}) => {
        const ws = {
          close: () => {
            if (handlers.onClose) handlers.onClose()
          },
          socketGetter: () => ({
            close: () => {
              if (handlers.onClose) handlers.onClose()
            },
            send: (data) => {
              if (handlers.onMessage) handlers.onMessage({ data })
            }
          })
        }

        if (handlers.onOpen) handlers.onOpen()
        return ws
      })
    },
    BsMessage: blockStub,
    BsCard: blockStub,
    BsFileUpload: blockStub,
    BsProgress: blockStub,
    BsInputBase: blockStub,
    BsInputText: blockStub,
    BsInputReadonly: blockStub,
    BsSelect: blockStub,
    BsInputTextArea: blockStub,
    BsInputNumber: blockStub,
    BsInputSwitch: blockStub,
    BsInputRadio: blockStub,
    BsDropdown: blockStub,
    BsModal: blockStub,
    BsModalConfirm: blockStub,
    BsInputTextAreaFormat: blockStub,
    BsMenuBar: blockStub,
    BsFooter: blockStub,
    BsInput: blockStub,
    BsButton: blockStub,
    IconHome: inlineStub,
    IconTools: inlineStub,
    IconGraphUpArrow: inlineStub,
    IconCloudUpArrow: inlineStub,
    IconUpArrow: inlineStub,
    IconCpu: inlineStub,
    IconWifi: inlineStub,
    IconEye: inlineStub,
    IconEyeSlash: inlineStub,
    IconCheckCircle: inlineStub,
    IconXCircle: inlineStub,
    IconExclamationTriangle: inlineStub,
    IconInfoCircle: inlineStub
  }
})

globalThis.import = {
  meta: {
    env: {
      VITE_APP_VERSION: '0.7.0-test',
      VITE_APP_BUILD: '1',
      BASE_URL: '/'
    }
  }
}

vtConfig.global = vtConfig.global || {}
vtConfig.global.components = vtConfig.global.components || {}

const uiStubs = [
  'BsInputReadonly',
  'BsInputNumber',
  'BsProgress',
  'BsFileUpload',
  'BsModalConfirm',
  'BsInput',
  'BsButton',
  'BsMessage',
  'BsCard',
  'BsInputText',
  'BsInputSwitch',
  'BsInputRadio',
  'BsDropdown',
  'BsModal',
  'BsInputTextAreaFormat',
  'BsSelect',
  'BsMenuBar',
  'BsFooter',
  'router-link',
  'router-view',
  'VoltageFragment',
  'ListFilesFragment',
  'AdvancedFilesFragment',
  'EnableCorsFragment'
]

uiStubs.forEach((name) => {
  vtConfig.global.components[name] = {
    template: '<div />'
  }
})
