import { beforeEach, describe, expect, it, vi } from 'vitest'

const mocks = vi.hoisted(() => ({
  config: {
    $id: 'config',
    mdns: 'pressuremon-test',
    wifi_ssid: 'ssid-1',
    dark_mode: false,
    helper: vi.fn(),
    $subscribe: vi.fn()
  },
  global: {
    initialized: true,
    configChanged: false
  },
  status: {},
  logInfo: vi.fn()
}))

vi.mock('@/modules/configStore', () => ({
  useConfigStore: vi.fn(() => mocks.config)
}))

vi.mock('@/modules/globalStore', () => ({
  useGlobalStore: vi.fn(() => mocks.global)
}))

vi.mock('@/modules/statusStore', () => ({
  useStatusStore: vi.fn(() => mocks.status)
}))

vi.mock('@mp-se/espframework-ui-components', () => ({
  logInfo: mocks.logInfo
}))

describe('pinia.js', () => {
  beforeEach(() => {
    vi.resetModules()
    mocks.config.$subscribe.mockClear()
    mocks.config.mdns = 'pressuremon-test'
    mocks.config.wifi_ssid = 'ssid-1'
    mocks.config.dark_mode = false
    mocks.global.initialized = true
    mocks.global.configChanged = false
  })

  it('exports singleton stores and helpers', async () => {
    const module = await import('../pinia')

    expect(module.default).toBeDefined()
    expect(module.config).toBe(mocks.config)
    expect(module.global).toBe(mocks.global)
    expect(module.status).toBe(mocks.status)
    expect(typeof module.saveConfigState).toBe('function')
    expect(typeof module.getConfigChanges).toBe('function')
  })

  it('getConfigChanges returns an empty object before saveConfigState', async () => {
    const module = await import('../pinia')
    expect(module.getConfigChanges()).toEqual({})
  })

  it('saveConfigState snapshots non-function config values', async () => {
    const module = await import('../pinia')

    module.saveConfigState()
    mocks.config.mdns = 'changed-name'

    expect(module.getConfigChanges()).toEqual({ mdns: 'changed-name' })
    expect(mocks.global.configChanged).toBe(false)
  })

  it('subscribe callback ignores changes before initialization', async () => {
    const module = await import('../pinia')
    module.saveConfigState()
    mocks.global.initialized = false
    mocks.config.mdns = 'changed-name'

    const callback = mocks.config.$subscribe.mock.calls[0][0]
    callback()

    expect(mocks.global.configChanged).toBe(false)
  })

  it('subscribe callback sets configChanged when snapshot differs', async () => {
    const module = await import('../pinia')
    module.saveConfigState()
    mocks.config.wifi_ssid = 'ssid-2'

    const callback = mocks.config.$subscribe.mock.calls[0][0]
    callback()

    expect(mocks.global.configChanged).toBe(true)
  })
})
