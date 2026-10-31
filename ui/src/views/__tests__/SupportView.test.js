import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import { flushPromises, mount } from '@vue/test-utils'

const mocks = vi.hoisted(() => ({
  global: {
    disabled: false,
    messageSuccess: '',
    clearMessages: vi.fn(),
    platform: 'ESP32',
    app_ver: '0.7.0',
    app_build: '42',
    uiVersion: '0.7.0-ui',
    uiBuild: '101'
  },
  config: {
    sendFilesystemRequest: vi.fn(),
    runHardwareScan: vi.fn()
  },
  logDebug: vi.fn()
}))

vi.mock('@/modules/pinia', () => ({
  global: mocks.global,
  config: mocks.config
}))

vi.mock('@mp-se/espframework-ui-components', () => ({
  logDebug: mocks.logDebug
}))

import SupportView from '../SupportView.vue'

function mountView() {
  return mount(SupportView)
}

describe('SupportView', () => {
  beforeEach(() => {
    mocks.global.disabled = false
    mocks.global.messageSuccess = ''
    mocks.global.clearMessages.mockReset()
    mocks.config.sendFilesystemRequest.mockReset()
    mocks.config.runHardwareScan.mockReset()
    mocks.logDebug.mockReset()
  })

  afterEach(() => {
    vi.restoreAllMocks()
  })

  it('renders the current PressureMon support links and version badges', () => {
    const wrapper = mountView()

    expect(wrapper.text()).toContain('Links and device logs')
    expect(wrapper.text()).toContain('Report issues on github.com')
    expect(wrapper.text()).toContain('Discuss on homebrewtalk.com')
    expect(wrapper.text()).toContain('ESP32')
    expect(wrapper.text()).toContain('0.7.0 (42)')
    expect(wrapper.text()).toContain('0.7.0-ui (101)')

    const links = wrapper.findAll('a[target="_blank"]')
    expect(links).toHaveLength(2)
    expect(links[0].attributes('href')).toBe('https://github.com/mp-se/pressuremon')
    expect(links[1].attributes('href')).toBe('https://www.homebrewtalk.com/')
  })

  it('toggles the error help section from the button', async () => {
    const wrapper = mountView()
    const button = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Toggle error help'))

    expect(wrapper.text()).not.toContain('400 - Bad request')
    await button.trigger('click')
    await flushPromises()
    expect(wrapper.text()).toContain('400 - Bad request')
    expect(wrapper.text()).toContain('-10 - Connection denied')

    await button.trigger('click')
    await flushPromises()
    expect(wrapper.text()).not.toContain('400 - Bad request')
  })

  it('views device logs in newest-first order across both files', async () => {
    mocks.config.sendFilesystemRequest.mockImplementation((data, callback) => {
      if (data.file === '/error2.log') callback(true, 'older-a\nolder-b')
      if (data.file === '/error.log') callback(true, 'newer-a\nnewer-b')
      return Promise.resolve({ success: true })
    })

    const wrapper = mountView()
    const button = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('View device logs'))
    await button.trigger('click')
    await flushPromises()

    expect(mocks.global.clearMessages).toHaveBeenCalled()
    expect(mocks.config.sendFilesystemRequest).toHaveBeenNthCalledWith(
      1,
      {
        command: 'get',
        file: '/error2.log'
      },
      expect.any(Function)
    )
    expect(mocks.config.sendFilesystemRequest).toHaveBeenNthCalledWith(
      2,
      {
        command: 'get',
        file: '/error.log'
      },
      expect.any(Function)
    )

    const text = wrapper.get('pre').text()
    expect(text).toContain('newer-b')
    expect(text.indexOf('newer-b')).toBeLessThan(text.indexOf('older-a'))
    expect(mocks.global.disabled).toBe(false)
  })

  it('stops loading even when log fetch callbacks report failure', async () => {
    mocks.config.sendFilesystemRequest.mockImplementation((data, callback) => {
      callback(false, '')
      return Promise.resolve({ success: false })
    })

    const wrapper = mountView()
    const button = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('View device logs'))
    await button.trigger('click')
    await flushPromises()

    expect(mocks.config.sendFilesystemRequest).toHaveBeenCalledTimes(2)
    expect(wrapper.get('pre').text()).toBe('')
    expect(mocks.global.disabled).toBe(false)
  })

  it('removes both log files and reports success', async () => {
    mocks.config.sendFilesystemRequest.mockImplementation((data, callback) => {
      callback(true)
      return Promise.resolve({ success: true })
    })

    const wrapper = mountView()
    const button = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Erase device logs'))
    await button.trigger('click')
    await flushPromises()

    expect(mocks.global.clearMessages).toHaveBeenCalled()
    expect(mocks.config.sendFilesystemRequest).toHaveBeenNthCalledWith(
      1,
      {
        command: 'del',
        file: '/error2.log'
      },
      expect.any(Function)
    )
    expect(mocks.config.sendFilesystemRequest).toHaveBeenNthCalledWith(
      2,
      {
        command: 'del',
        file: '/error.log'
      },
      expect.any(Function)
    )
    expect(mocks.global.messageSuccess).toBe('Requested logs to be deleted')
    expect(mocks.global.disabled).toBe(false)
  })

  it('loads hardware scan output into the log area', async () => {
    mocks.config.runHardwareScan.mockResolvedValueOnce('sensor-a\nsensor-b')

    const wrapper = mountView()
    const button = wrapper.findAll('button').find((entry) => entry.text().includes('Hardware scan'))
    await button.trigger('click')
    await flushPromises()

    expect(mocks.global.clearMessages).toHaveBeenCalled()
    expect(mocks.config.runHardwareScan).toHaveBeenCalled()
    expect(wrapper.get('pre').text()).toBe('sensor-a\nsensor-b')
    expect(mocks.global.disabled).toBe(false)
  })

  it('clears disabled state when hardware scan throws', async () => {
    mocks.config.runHardwareScan.mockRejectedValueOnce(new Error('scan failed'))

    const wrapper = mountView()
    const button = wrapper.findAll('button').find((entry) => entry.text().includes('Hardware scan'))
    await button.trigger('click')
    await flushPromises()

    expect(wrapper.get('pre').text()).toBe('')
    expect(mocks.global.disabled).toBe(false)
  })
})
