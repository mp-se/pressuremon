vi.doUnmock('../statusStore')

import { beforeEach, describe, expect, it, vi } from 'vitest'
import { createPinia, setActivePinia } from 'pinia'

const mocks = vi.hoisted(() => ({
  http: {
    auth: vi.fn(),
    request: vi.fn()
  },
  global: {
    fetchTimeout: 5000,
    id: 'device-123'
  }
}))

vi.mock('@/modules/pinia', () => ({
  global: mocks.global
}))

vi.mock('@mp-se/espframework-ui-components', () => ({
  logDebug: vi.fn(),
  logError: vi.fn(),
  logInfo: vi.fn(),
  sharedHttpClient: mocks.http
}))

describe('statusStore', () => {
  let store

  beforeEach(async () => {
    setActivePinia(createPinia())
    mocks.http.auth.mockReset()
    mocks.http.request.mockReset()

    const module = await import('../statusStore')
    store = module.useStatusStore()
  })

  it('initializes with pressure defaults and unit getters', () => {
    expect(store.pressure).toBe(10)
    expect(store.pressure1).toBe(11)
    expect(store.isPsi).toBe(true)
    expect(store.isBar).toBe(false)
    expect(store.isKPa).toBe(false)
  })

  it('unit getters switch for Bar and kPa pressure units', () => {
    store.pressure_unit = 'Bar'
    expect(store.isPsi).toBe(false)
    expect(store.isBar).toBe(true)
    expect(store.isKPa).toBe(false)

    store.pressure_unit = 'kPa'
    expect(store.isPsi).toBe(false)
    expect(store.isBar).toBe(false)
    expect(store.isKPa).toBe(true)
  })

  it('authAsync returns the shared client token when authentication succeeds', async () => {
    mocks.http.auth.mockResolvedValueOnce(true)
    mocks.http.token = 'token-123'

    await expect(store.authAsync()).resolves.toEqual({
      success: true,
      data: { token: 'token-123' }
    })
  })

  it('authAsync falls back to global.id when the shared client token is missing', async () => {
    mocks.http.auth.mockResolvedValueOnce(true)
    mocks.http.token = ''

    await expect(store.authAsync()).resolves.toEqual({
      success: true,
      data: { token: 'device-123' }
    })
  })

  it('authAsync returns null token when neither shared token nor global id exist', async () => {
    mocks.http.auth.mockResolvedValueOnce(true)
    mocks.http.token = ''
    mocks.global.id = ''

    await expect(store.authAsync()).resolves.toEqual({
      success: true,
      data: { token: null }
    })

    mocks.global.id = 'device-123'
  })

  it('authAsync returns failure when authentication is rejected', async () => {
    mocks.http.auth.mockResolvedValueOnce(false)
    await expect(store.authAsync()).resolves.toEqual({ success: false, data: null })
  })

  it('load normalizes and rounds status values', async () => {
    mocks.http.request.mockResolvedValueOnce({
      json: async () => ({
        id: 'device-1',
        pressure: 10.456,
        pressure1: 11.234,
        pressure_unit: 'PSI',
        temp: 21.678,
        temp_unit: 'C',
        sleep_mode: true,
        battery: 3.756,
        rssi: -55,
        mdns: 'pressuremon-test',
        wifi_ssid: 'ssid-1',
        ip: '192.168.1.10',
        self_check: {
          sensor_connected: false,
          sensor_configured: true,
          battery_level: false,
          push_targets: true
        },
        total_heap: 204800,
        free_heap: 102400,
        wifi_setup: true,
        max_sensors: 2,
        adc_found: true
      })
    })

    const result = await store.load()

    expect(result).toBe(true)
    expect(store.id).toBe('device-1')
    expect(store.pressure).toBe('10.46')
    expect(store.pressure1).toBe('11.23')
    expect(store.temp).toBe('21.68')
    expect(store.battery).toBe('3.76')
    expect(store.total_heap).toBe('200')
    expect(store.free_heap).toBe('100')
    expect(store.self_check.sensor_connected).toBe(false)
    expect(store.wifi_setup).toBe(true)
    expect(store.max_sensors).toBe(2)
    expect(store.adc_found).toBe(true)
  })

  it('load returns false on request errors', async () => {
    mocks.http.request.mockRejectedValueOnce(new Error('network'))
    await expect(store.load()).resolves.toBe(false)
  })

  it('load skips rounding branches when pressure fields are undefined', async () => {
    mocks.http.request.mockResolvedValueOnce({
      json: async () => ({
        id: 'device-2',
        pressure: undefined,
        pressure1: undefined,
        pressure_unit: 'kPa',
        temp: undefined,
        temp_unit: 'C',
        sleep_mode: false,
        battery: 3.1,
        rssi: -65,
        mdns: 'pressuremon-skip',
        wifi_ssid: 'ssid-2',
        ip: '192.168.1.11',
        self_check: {
          sensor_connected: true,
          sensor_configured: false,
          battery_level: true,
          push_targets: false
        },
        total_heap: 1024,
        free_heap: 512,
        wifi_setup: false,
        max_sensors: 1,
        adc_found: false
      })
    })

    const result = await store.load()

    expect(result).toBe(true)
    expect(store.pressure).toBeUndefined()
    expect(store.pressure1).toBeUndefined()
    expect(store.temp).toBeUndefined()
    expect(store.battery).toBe('3.10')
    expect(store.isKPa).toBe(true)
  })
})
