vi.doUnmock('../globalStore')

import { beforeEach, describe, expect, it, vi } from 'vitest'
import { createPinia, setActivePinia } from 'pinia'

const { httpMock } = vi.hoisted(() => ({
  httpMock: {
    getJson: vi.fn()
  }
}))

vi.mock('@mp-se/espframework-ui-components', () => ({
  logInfo: vi.fn(),
  logError: vi.fn(),
  logDebug: vi.fn(),
  sharedHttpClient: httpMock
}))

describe('globalStore', () => {
  let store

  beforeEach(async () => {
    setActivePinia(createPinia())
    httpMock.getJson.mockReset()

    const module = await import('../globalStore')
    store = module.useGlobalStore()
  })

  it('initializes with pressuremon defaults', () => {
    expect(store.platform).toBe('')
    expect(store.ui.enableVoltageFragment).toBe(true)
    expect(store.ui.enableManualWifiEntry).toBe(false)
    expect(store.feature.ble).toBe(false)
    expect(store.feature.no_sensors).toBe(0)
    expect(store.initialized).toBe(false)
  })

  it('clearMessages resets all message fields', () => {
    store.messageError = 'error'
    store.messageWarning = 'warning'
    store.messageSuccess = 'success'
    store.messageInfo = 'info'

    store.clearMessages()

    expect(store.messageError).toBe('')
    expect(store.messageWarning).toBe('')
    expect(store.messageSuccess).toBe('')
    expect(store.messageInfo).toBe('')
  })

  it('message getters reflect the current message state', () => {
    store.messageError = 'error'
    store.messageWarning = 'warning'
    store.messageSuccess = 'success'
    store.messageInfo = 'info'

    expect(store.isError).toBe(true)
    expect(store.isWarning).toBe(true)
    expect(store.isSuccess).toBe(true)
    expect(store.isInfo).toBe(true)
  })

  it('message getters return false when messages are empty', () => {
    store.messageError = ''
    store.messageWarning = ''
    store.messageSuccess = ''
    store.messageInfo = ''

    expect(store.isError).toBe(false)
    expect(store.isWarning).toBe(false)
    expect(store.isSuccess).toBe(false)
    expect(store.isInfo).toBe(false)
  })

  it('ui getters expose build metadata', () => {
    expect(['string', 'undefined']).toContain(typeof store.uiVersion)
    expect(['string', 'undefined']).toContain(typeof store.uiBuild)
  })

  it('load populates feature data and normalizes casing', async () => {
    httpMock.getJson.mockResolvedValueOnce({
      board: 'esp32-devkit',
      app_ver: '0.7.0',
      app_build: '42',
      platform: 'esp32',
      firmware_file: 'PRESSUREMON.BIN',
      ble: true,
      no_sensors: 2
    })

    const result = await store.load()

    expect(result).toBe(true)
    expect(store.board).toBe('ESP32-DEVKIT')
    expect(store.app_ver).toBe('0.7.0')
    expect(store.app_build).toBe('42')
    expect(store.platform).toBe('ESP32')
    expect(store.firmware_file).toBe('pressuremon.bin')
    expect(store.feature.ble).toBe(true)
    expect(store.feature.no_sensors).toBe(2)
  })

  it('load returns false on fetch errors', async () => {
    httpMock.getJson.mockRejectedValueOnce(new Error('network'))
    await expect(store.load()).resolves.toBe(false)
  })
})
