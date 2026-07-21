vi.doUnmock('../configStore')

import { beforeEach, describe, expect, it, vi } from 'vitest'
import { createPinia, setActivePinia } from 'pinia'

const mocks = vi.hoisted(() => ({
  http: {
    getJson: vi.fn(),
    postJson: vi.fn(),
    request: vi.fn(),
    restart: vi.fn(),
    getErrorString: vi.fn((code) => `Error ${code}`),
    filesystemRequest: vi.fn()
  },
  global: {
    disabled: false,
    initialized: true,
    configChanged: false,
    messageError: '',
    messageWarning: '',
    messageSuccess: '',
    clearMessages: vi.fn()
  },
  saveConfigState: vi.fn(),
  getConfigChanges: vi.fn(() => ({}))
}))

vi.mock('@mp-se/espframework-ui-components', () => ({
  logDebug: vi.fn(),
  logError: vi.fn(),
  logInfo: vi.fn(),
  tempToC: (value) => ((value - 32) * 5) / 9,
  tempToF: (value) => (value * 9) / 5 + 32,
  roundVal: (value, digits) => parseFloat(Number(value).toFixed(digits)),
  sharedHttpClient: mocks.http
}))

vi.mock('@/modules/pinia', () => ({
  global: mocks.global,
  saveConfigState: mocks.saveConfigState,
  getConfigChanges: mocks.getConfigChanges
}))

describe('configStore', () => {
  let store

  beforeEach(async () => {
    setActivePinia(createPinia())
    vi.clearAllMocks()

    mocks.global.disabled = false
    mocks.global.messageError = ''
    mocks.global.messageWarning = ''
    mocks.global.messageSuccess = ''

    const module = await import('../configStore')
    store = module.useConfigStore()
    store.formula_calibration_temp = 20
    store.internal_temp_unit = 'C'
  })

  it('creates the config store', () => {
    expect(store.$id).toBe('config')
  })

  it('converts temperatures between C and F', () => {
    store.temp_adjustment_value = 1
    store.temp_unit = 'F'
    store.convertTemp()

    expect(store.internal_temp_unit).toBe('F')
    expect(store.formula_calibration_temp).toBe(68)
    expect(store.temp_adjustment_value).toBe(1.8)

    store.convertTempToC()
    expect(store.internal_temp_unit).toBe('C')
    expect(store.formula_calibration_temp).toBe(20)
    expect(store.temp_adjustment_value).toBe(1)
  })

  it('toJson serializes public state fields only', () => {
    store.mdns = 'pressuremon-json'
    store.dark_mode = true

    const json = JSON.parse(store.toJson())

    expect(json.mdns).toBe('pressuremon-json')
    expect(json.dark_mode).toBe(true)
    expect(json.$id).toBeUndefined()
  })

  it('load populates pressuremon config fields', async () => {
    mocks.http.getJson.mockResolvedValueOnce({
      id: 'device-1',
      mdns: 'pressuremon-test',
      temp_unit: 'C',
      voltage_factor: 1.1,
      pressure_unit: 'bar',
      wifi_ssid: 'ssid-1',
      sleep_interval: 60,
      mqtt_target: 'mqtt://broker',
      ble_format: 1,
      dark_mode: true
    })

    const result = await store.load()

    expect(result).toBe(true)
    expect(store.id).toBe('device-1')
    expect(store.mdns).toBe('pressuremon-test')
    expect(store.voltage_factor).toBe(1.1)
    expect(store.pressure_unit).toBe('bar')
    expect(store.wifi_ssid).toBe('ssid-1')
    expect(store.mqtt_target).toBe('mqtt://broker')
    expect(store.ble_format).toBe(1)
    expect(store.dark_mode).toBe(true)
  })

  it('load returns false on fetch errors', async () => {
    mocks.http.getJson.mockRejectedValueOnce(new Error('load failed'))
    await expect(store.load()).resolves.toBe(false)
  })

  it('loadFormat decodes pressure format templates', async () => {
    mocks.http.getJson.mockResolvedValueOnce({
      http_post_format_pressure: encodeURIComponent('{pressure}'),
      http_post2_format_pressure: encodeURIComponent('{pressure1}'),
      http_get_format_pressure: encodeURIComponent('pressure={pressure}'),
      influxdb2_format_pressure: encodeURIComponent('pressure value={pressure}'),
      mqtt_format_pressure: encodeURIComponent('pressure|battery')
    })

    await expect(store.loadFormat()).resolves.toBe(true)
    expect(store.http_post_format_pressure).toBe('{pressure}')
    expect(store.http_post2_format_pressure).toBe('{pressure1}')
    expect(store.http_get_format_pressure).toBe('pressure={pressure}')
    expect(store.influxdb2_format_pressure).toBe('pressure value={pressure}')
    expect(store.mqtt_format_pressure).toBe('pressure|\nbattery')
  })

  it('loadFormat returns false on fetch errors', async () => {
    mocks.http.getJson.mockRejectedValueOnce(new Error('format failed'))
    await expect(store.loadFormat()).resolves.toBe(false)
  })

  it('sendConfig removes format fields before posting changes', async () => {
    mocks.getConfigChanges.mockReturnValueOnce({
      mdns: 'pressuremon-new',
      http_post_format_pressure: '{pressure}'
    })

    await expect(store.sendConfig()).resolves.toBe(true)
    expect(mocks.http.postJson).toHaveBeenCalledWith('api/config', { mdns: 'pressuremon-new' })
  })

  it('sendConfig returns true immediately when there are no changes', async () => {
    mocks.getConfigChanges.mockReturnValueOnce({})
    await expect(store.sendConfig()).resolves.toBe(true)
    expect(mocks.http.postJson).not.toHaveBeenCalled()
  })

  it('sendConfig returns false when posting config fails', async () => {
    mocks.getConfigChanges.mockReturnValueOnce({ mdns: 'pressuremon-new' })
    mocks.http.postJson.mockRejectedValueOnce(new Error('config failed'))
    await expect(store.sendConfig()).resolves.toBe(false)
  })

  it('sendFormat encodes pressure format fields and strips mqtt newlines', async () => {
    mocks.getConfigChanges.mockReturnValue({
      http_post_format_pressure: '{pressure}',
      mqtt_format_pressure: 'pressure|\nvalue\r\n'
    })

    await expect(store.sendFormat()).resolves.toBe(true)
    expect(mocks.http.postJson).toHaveBeenCalledWith('api/format', {
      http_post_format_pressure: encodeURIComponent('{pressure}')
    })
    expect(mocks.http.postJson).toHaveBeenCalledWith('api/format', {
      mqtt_format_pressure: encodeURIComponent('pressure|value')
    })
  })

  it('sendFormat returns false when any format upload fails', async () => {
    mocks.getConfigChanges.mockReturnValue({
      http_post_format_pressure: '{pressure}',
      http_get_format_pressure: 'pressure={pressure}'
    })

    vi.spyOn(store, 'sendOneFormat')
      .mockResolvedValueOnce(true)
      .mockResolvedValueOnce(true)
      .mockResolvedValueOnce(false)
      .mockResolvedValueOnce(true)
      .mockResolvedValueOnce(true)

    await expect(store.sendFormat()).resolves.toBe(false)
  })

  it('getPushTestStatus returns the push status payload', async () => {
    mocks.http.getJson.mockResolvedValueOnce({ status: false, success: true })
    await expect(store.getPushTestStatus()).resolves.toEqual({
      success: true,
      data: { status: false, success: true }
    })
  })

  it('getPushTestStatus returns failure on fetch errors', async () => {
    mocks.http.getJson.mockRejectedValueOnce(new Error('push status failed'))
    await expect(store.getPushTestStatus()).resolves.toEqual({ success: false, data: null })
  })

  it('sendWifiScan returns true on request success', async () => {
    mocks.http.request.mockResolvedValueOnce({})
    await expect(store.sendWifiScan()).resolves.toBe(true)
  })

  it('sendWifiScan returns false on request failure', async () => {
    mocks.http.request.mockRejectedValueOnce(new Error('wifi failed'))
    await expect(store.sendWifiScan()).resolves.toBe(false)
  })

  it('getWifiScanStatus returns status payload on success', async () => {
    mocks.http.getJson.mockResolvedValueOnce({ status: false, success: true })
    await expect(store.getWifiScanStatus()).resolves.toEqual({
      success: true,
      data: { status: false, success: true }
    })
  })

  it('getWifiScanStatus returns failure on errors', async () => {
    mocks.http.getJson.mockRejectedValueOnce(new Error('wifi status failed'))
    await expect(store.getWifiScanStatus()).resolves.toEqual({ success: false, data: null })
  })

  it('sendHardwareScan returns true on request success', async () => {
    mocks.http.request.mockResolvedValueOnce({})
    await expect(store.sendHardwareScan()).resolves.toBe(true)
  })

  it('sendHardwareScan returns false on request failure', async () => {
    mocks.http.request.mockRejectedValueOnce(new Error('hardware failed'))
    await expect(store.sendHardwareScan()).resolves.toBe(false)
  })

  it('getHardwareScanStatus returns status payload on success', async () => {
    mocks.http.getJson.mockResolvedValueOnce({ status: false, success: true })
    await expect(store.getHardwareScanStatus()).resolves.toEqual({
      success: true,
      data: { status: false, success: true }
    })
  })

  it('getHardwareScanStatus returns failure on errors', async () => {
    mocks.http.getJson.mockRejectedValueOnce(new Error('hardware status failed'))
    await expect(store.getHardwareScanStatus()).resolves.toEqual({ success: false, data: null })
  })

  it('setSleepMode returns false for non-ok responses', async () => {
    mocks.http.request.mockResolvedValueOnce({
      ok: false,
      status: 403,
      statusText: 'Forbidden',
      text: async () => 'nope'
    })

    await expect(store.setSleepMode(true)).resolves.toBe(false)
  })

  it('restart reports success and failure states', async () => {
    store.mdns = 'pressuremon-test'
    mocks.http.restart.mockResolvedValueOnce({
      success: true,
      json: { status: true, message: 'Restarting' }
    })

    await store.restart()
    expect(mocks.global.messageSuccess).toContain('Restarting')
    expect(mocks.global.messageSuccess).toContain('pressuremon-test.local')

    mocks.http.restart.mockResolvedValueOnce({ success: false })
    await store.restart()
    expect(mocks.global.messageError).toBe('Failed to request restart')
  })

  it('saveAll persists config and format changes', async () => {
    vi.spyOn(store, 'sendConfig').mockResolvedValueOnce(true)
    vi.spyOn(store, 'sendFormat').mockResolvedValueOnce(true)

    await store.saveAll()

    expect(mocks.global.messageSuccess).toBe('Configuration has been saved to device')
    expect(mocks.saveConfigState).toHaveBeenCalled()
  })

  it('sendFilesystemRequest forwards the callback response', async () => {
    const callback = vi.fn()
    mocks.http.filesystemRequest.mockResolvedValueOnce({ success: true, text: 'log text' })

    const result = await store.sendFilesystemRequest(
      { command: 'get', file: '/error.log' },
      callback
    )

    expect(mocks.http.filesystemRequest).toHaveBeenCalledWith({
      command: 'get',
      file: '/error.log'
    })
    expect(callback).toHaveBeenCalledWith(true, 'log text')
    expect(result).toEqual({ success: true, text: 'log text' })
  })

  it('sendOneFormat skips empty payloads', async () => {
    await expect(store.sendOneFormat({})).resolves.toBe(true)
    expect(mocks.http.postJson).not.toHaveBeenCalled()
  })

  it('sendOneFormat returns false when posting fails', async () => {
    mocks.http.postJson.mockRejectedValueOnce(new Error('format failed'))
    await expect(store.sendOneFormat({ http_get_format_pressure: 'x' })).resolves.toBe(false)
  })

  it('sendPushTest returns false and resets disabled on errors', async () => {
    mocks.http.postJson.mockRejectedValueOnce(new Error('push failed'))
    await expect(store.sendPushTest({ push_format: 'mqtt_format_pressure' })).resolves.toBe(false)
    expect(mocks.global.disabled).toBe(false)
  })

  it('sendPushTest returns true on successful post', async () => {
    mocks.http.postJson.mockResolvedValueOnce({})
    await expect(store.sendPushTest({ push_format: 'mqtt_format_pressure' })).resolves.toBe(true)
    expect(mocks.global.disabled).toBe(false)
  })

  it('sendFilesystemRequest returns failure when the filesystem call throws', async () => {
    const callback = vi.fn()
    mocks.http.filesystemRequest.mockRejectedValueOnce(new Error('fs failed'))

    await expect(
      store.sendFilesystemRequest({ command: 'get', file: '/error.log' }, callback)
    ).resolves.toEqual({ success: false, text: '' })

    expect(callback).toHaveBeenCalledWith(false, '')
    expect(mocks.global.disabled).toBe(false)
  })

  it('setSleepMode returns true for ok responses even when json parsing fails', async () => {
    mocks.http.request.mockResolvedValueOnce({
      ok: true,
      json: async () => {
        throw new Error('bad json')
      }
    })

    await expect(store.setSleepMode(false)).resolves.toBe(true)
  })

  it('setSleepMode returns false when the request throws', async () => {
    mocks.http.request.mockRejectedValueOnce(new Error('sleep failed'))
    await expect(store.setSleepMode(true)).resolves.toBe(false)
  })

  it('restart surfaces message from unsuccessful json responses', async () => {
    mocks.http.restart.mockResolvedValueOnce({
      success: true,
      json: { status: false, message: 'Denied' }
    })

    await store.restart()
    expect(mocks.global.messageError).toBe('Denied')
  })

  it('restart handles thrown errors', async () => {
    mocks.http.restart.mockRejectedValueOnce(new Error('restart failed'))
    await store.restart()
    expect(mocks.global.messageError).toBe('Failed to do restart')
  })

  it('saveAll reports config save failures', async () => {
    vi.spyOn(store, 'sendConfig').mockResolvedValueOnce(false)
    await store.saveAll()
    expect(mocks.global.messageError).toBe('Failed to store configuration to device')
  })

  it('saveAll reports format save failures', async () => {
    vi.spyOn(store, 'sendConfig').mockResolvedValueOnce(true)
    vi.spyOn(store, 'sendFormat').mockResolvedValueOnce(false)
    await store.saveAll()
    expect(mocks.global.messageError).toBe('Failed to store format to device')
  })

  it('saveAll reports unexpected exceptions', async () => {
    vi.spyOn(store, 'sendConfig').mockRejectedValueOnce(new Error('explode'))
    await store.saveAll()
    expect(mocks.global.messageError).toBe('Failed to save configuration')
    expect(mocks.global.disabled).toBe(false)
  })

  it('runPushTest reports start failures', async () => {
    vi.spyOn(store, 'sendPushTest').mockResolvedValueOnce(false)
    await expect(store.runPushTest({ push_format: 'http_get_format_pressure' })).resolves.toBe(
      false
    )
    expect(mocks.global.messageError).toBe('Failed to start push test')
  })

  it('runPushTest reports polling failures', async () => {
    vi.spyOn(store, 'sendPushTest').mockResolvedValueOnce(true)
    vi.spyOn(store, 'getPushTestStatus').mockResolvedValueOnce({ success: false, data: null })

    await expect(store.runPushTest({ push_format: 'http_get_format_pressure' })).resolves.toBe(
      false
    )
    expect(mocks.global.messageError).toBe('Failed to get push test status')
  })

  it('runPushTest reports target-side failures with a return code', async () => {
    vi.spyOn(store, 'sendPushTest').mockResolvedValueOnce(true)
    vi.spyOn(store, 'getPushTestStatus').mockResolvedValueOnce({
      success: true,
      data: { status: false, success: false, push_return_code: 7 }
    })

    await expect(store.runPushTest({ push_format: 'http_get_format_pressure' })).resolves.toBe(true)
    expect(mocks.global.messageError).toBe('Test failed with error code (7)')
  })

  it('runPushTest warns when no endpoint is enabled', async () => {
    vi.spyOn(store, 'sendPushTest').mockResolvedValueOnce(true)
    vi.spyOn(store, 'getPushTestStatus').mockResolvedValueOnce({
      success: true,
      data: { status: false, success: true, push_enabled: false, push_return_code: 0 }
    })

    await expect(store.runPushTest({ push_format: 'mqtt_format_pressure' })).resolves.toBe(true)
    expect(mocks.global.messageWarning).toBe(
      'No endpoint is defined for this target. Cannot run test.'
    )
  })

  it('runPushTest reports successful completion', async () => {
    vi.spyOn(store, 'sendPushTest').mockResolvedValueOnce(true)
    vi.spyOn(store, 'getPushTestStatus').mockResolvedValueOnce({
      success: true,
      data: { status: false, success: true, push_enabled: true, push_return_code: 0 }
    })

    await expect(store.runPushTest({ push_format: 'mqtt_format_pressure' })).resolves.toBe(true)
    expect(mocks.global.messageSuccess).toBe('Test was successful')
  })

  it('runPushTest continues polling while the test is still running', async () => {
    vi.spyOn(store, 'sendPushTest').mockResolvedValueOnce(true)
    vi.spyOn(globalThis, 'setTimeout').mockImplementation((fn) => {
      fn()
      return 0
    })
    vi.spyOn(store, 'getPushTestStatus')
      .mockResolvedValueOnce({ success: true, data: { status: true } })
      .mockResolvedValueOnce({
        success: true,
        data: { status: false, success: true, push_enabled: true, push_return_code: 0 }
      })

    await expect(store.runPushTest({ push_format: 'mqtt_format_pressure' })).resolves.toBe(true)
    expect(mocks.global.messageSuccess).toBe('Test was successful')
  })

  it('runPushTest reports ssl skip errors when the test never starts', async () => {
    vi.spyOn(store, 'sendPushTest').mockResolvedValueOnce(true)
    vi.spyOn(store, 'getPushTestStatus').mockResolvedValueOnce({
      success: true,
      data: { status: false, success: false, push_enabled: true, push_return_code: 0 }
    })

    await expect(store.runPushTest({ push_format: 'mqtt_format_pressure' })).resolves.toBe(true)
    expect(mocks.global.messageError).toContain('Test failed with error code (0)')
  })

  it('runPushTest handles unexpected exceptions', async () => {
    vi.spyOn(store, 'sendPushTest').mockRejectedValueOnce(new Error('unexpected'))
    await expect(store.runPushTest({ push_format: 'mqtt_format_pressure' })).resolves.toBe(false)
    expect(mocks.global.messageError).toBe('Push test failed unexpectedly')
  })

  it('runWifiScan reports start failures', async () => {
    vi.spyOn(store, 'sendWifiScan').mockResolvedValueOnce(false)
    await expect(store.runWifiScan()).resolves.toEqual({ success: false })
    expect(mocks.global.messageError).toBe('Failed to start wifi scan')
  })

  it('runWifiScan reports polling failures', async () => {
    vi.spyOn(store, 'sendWifiScan').mockResolvedValueOnce(true)
    vi.spyOn(store, 'getWifiScanStatus').mockResolvedValueOnce({ success: false, data: null })
    await expect(store.runWifiScan()).resolves.toEqual({ success: false })
    expect(mocks.global.messageError).toBe('Failed to get wifi scan status')
  })

  it('runHardwareScan reports start failures', async () => {
    vi.spyOn(store, 'sendHardwareScan').mockResolvedValueOnce(false)
    await expect(store.runHardwareScan()).resolves.toEqual({ success: false })
    expect(mocks.global.messageError).toBe('Failed to start hardware scan')
  })

  it('runHardwareScan reports polling failures', async () => {
    vi.spyOn(store, 'sendHardwareScan').mockResolvedValueOnce(true)
    vi.spyOn(store, 'getHardwareScanStatus').mockResolvedValueOnce({ success: false, data: null })
    await expect(store.runHardwareScan()).resolves.toEqual({ success: false })
    expect(mocks.global.messageError).toBe('Failed to get hardware scan status')
  })

  it('runWifiScan returns the completed scan payload', async () => {
    vi.spyOn(store, 'sendWifiScan').mockResolvedValueOnce(true)
    vi.spyOn(store, 'getWifiScanStatus').mockResolvedValueOnce({
      success: true,
      data: { status: false, success: true, networks: [{ wifi_ssid: 'ssid-1' }] }
    })

    await expect(store.runWifiScan()).resolves.toEqual({
      success: true,
      data: { status: false, success: true, networks: [{ wifi_ssid: 'ssid-1' }] }
    })
  })

  it('runWifiScan continues polling until scan completes', async () => {
    vi.spyOn(store, 'sendWifiScan').mockResolvedValueOnce(true)
    vi.spyOn(globalThis, 'setTimeout').mockImplementation((fn) => {
      fn()
      return 0
    })
    vi.spyOn(store, 'getWifiScanStatus')
      .mockResolvedValueOnce({ success: true, data: { status: true } })
      .mockResolvedValueOnce({
        success: true,
        data: { status: false, success: true, networks: [{ wifi_ssid: 'ssid-2' }] }
      })

    await expect(store.runWifiScan()).resolves.toEqual({
      success: true,
      data: { status: false, success: true, networks: [{ wifi_ssid: 'ssid-2' }] }
    })
  })

  it('runHardwareScan returns the completed scan payload', async () => {
    vi.spyOn(store, 'sendHardwareScan').mockResolvedValueOnce(true)
    vi.spyOn(store, 'getHardwareScanStatus').mockResolvedValueOnce({
      success: true,
      data: { status: false, success: true, devices: ['sensor-1'] }
    })

    await expect(store.runHardwareScan()).resolves.toEqual({
      success: true,
      data: { status: false, success: true, devices: ['sensor-1'] }
    })
  })

  it('runHardwareScan continues polling until scan completes', async () => {
    vi.spyOn(store, 'sendHardwareScan').mockResolvedValueOnce(true)
    vi.spyOn(globalThis, 'setTimeout').mockImplementation((fn) => {
      fn()
      return 0
    })
    vi.spyOn(store, 'getHardwareScanStatus')
      .mockResolvedValueOnce({ success: true, data: { status: true } })
      .mockResolvedValueOnce({
        success: true,
        data: { status: false, success: true, devices: ['sensor-2'] }
      })

    await expect(store.runHardwareScan()).resolves.toEqual({
      success: true,
      data: { status: false, success: true, devices: ['sensor-2'] }
    })
  })
})
