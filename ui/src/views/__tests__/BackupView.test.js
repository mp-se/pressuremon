import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import { flushPromises, mount } from '@vue/test-utils'

const mocks = vi.hoisted(() => ({
  global: {
    disabled: false,
    messageError: '',
    messageSuccess: '',
    clearMessages: vi.fn()
  },
  config: {
    mdns: 'pressuremon-test',
    http_post_format_pressure: '{pressure}',
    http_post2_format_pressure: '{pressure1}',
    http_get_format_pressure: 'pressure={pressure}',
    influxdb2_format_pressure: 'pressure value={pressure}',
    mqtt_format_pressure: 'pressure|\nvalue',
    toJson: vi.fn(),
    saveAll: vi.fn()
  },
  getConfigChanges: vi.fn(),
  logDebug: vi.fn(),
  logError: vi.fn()
}))

vi.mock('@/modules/pinia', () => ({
  global: mocks.global,
  config: mocks.config,
  getConfigChanges: mocks.getConfigChanges
}))

vi.mock('@mp-se/espframework-ui-components', () => ({
  BsFileUpload: {
    template:
      '<input id="upload" type="file" :disabled="disabled" @change="$emit(\'change\', $event)" />',
    props: ['disabled']
  },
  BsProgress: {
    template: '<div data-test="progress"></div>'
  },
  logDebug: mocks.logDebug,
  logError: mocks.logError
}))

import BackupView from '../BackupView.vue'

function mountView() {
  return mount(BackupView)
}

function installFileReader(result) {
  const instance = {
    addEventListener: vi.fn((event, callback) => {
      if (event === 'load') instance.onload = callback
    }),
    readAsText: vi.fn(() => {
      instance.onload({ target: { result } })
    })
  }

  vi.stubGlobal(
    'FileReader',
    vi.fn(() => instance)
  )
  return instance
}

describe('BackupView', () => {
  beforeEach(() => {
    mocks.global.disabled = false
    mocks.global.messageError = ''
    mocks.global.messageSuccess = ''
    mocks.config.mdns = 'pressuremon-test'
    mocks.config.http_post_format_pressure = '{pressure}'
    mocks.config.http_post2_format_pressure = '{pressure1}'
    mocks.config.http_get_format_pressure = 'pressure={pressure}'
    mocks.config.influxdb2_format_pressure = 'pressure value={pressure}'
    mocks.config.mqtt_format_pressure = 'pressure|\nvalue'
    mocks.config.toJson.mockImplementation(() =>
      JSON.stringify({
        mdns: mocks.config.mdns,
        http_post_format_pressure: mocks.config.http_post_format_pressure,
        http_post2_format_pressure: mocks.config.http_post2_format_pressure,
        http_get_format_pressure: mocks.config.http_get_format_pressure,
        influxdb2_format_pressure: mocks.config.influxdb2_format_pressure,
        mqtt_format_pressure: mocks.config.mqtt_format_pressure
      })
    )
    mocks.config.saveAll.mockImplementation(async () => {
      mocks.global.disabled = false
      return true
    })
    mocks.getConfigChanges.mockReset()
    mocks.logDebug.mockReset()
    mocks.logError.mockReset()
  })

  afterEach(() => {
    vi.restoreAllMocks()
    vi.unstubAllGlobals()
  })

  it('renders the current pressuremon backup workflow', () => {
    const wrapper = mountView()

    expect(wrapper.text()).toContain('Backup & Restore')
    const restoreButton = wrapper.find('button[type="submit"]')
    expect(restoreButton.attributes('title')).toBe('Please select a backup file first')
    expect(restoreButton.attributes('disabled')).toBeDefined()
  })

  it('enables restore after selecting a file', async () => {
    const wrapper = mountView()
    const input = wrapper.get('#upload')

    Object.defineProperty(input.element, 'files', {
      value: [new File(['backup'], 'backup.txt')],
      configurable: true
    })

    await input.trigger('change')

    const restoreButton = wrapper.get('button[type="submit"]')
    expect(restoreButton.attributes('title')).toBe('Upload the configuration to the device')
    expect(restoreButton.attributes('disabled')).toBeUndefined()
  })

  it('creates a PressureMon backup and encodes pressure formats', async () => {
    const wrapper = mountView()
    mocks.config.mdns = 'device-42'
    const blobSpy = vi.fn((parts, options) => ({ parts, type: options.type }))

    const clickSpy = vi.fn()
    vi.spyOn(document, 'createElement').mockReturnValue({
      setAttribute: vi.fn(),
      click: clickSpy
    })

    const createObjectURL = vi.fn(() => 'blob:backup')
    vi.stubGlobal('URL', {
      createObjectURL,
      revokeObjectURL: vi.fn()
    })
    vi.stubGlobal('Blob', blobSpy)

    await wrapper.get('button.btn.btn-primary.w-2').trigger('click')

    expect(mocks.config.toJson).toHaveBeenCalled()
    expect(clickSpy).toHaveBeenCalled()
    expect(mocks.global.messageSuccess).toContain('device-42.txt')

    const blob = createObjectURL.mock.calls[0][0]
    expect(blobSpy).toHaveBeenCalled()
    const payload = JSON.parse(blob.parts[0])
    expect(payload.meta.software).toBe('PressureMon')
    expect(payload.meta.version).toBe('0.5.0')
    expect(payload.config.http_post_format_pressure).toBe(encodeURIComponent('{pressure}'))
    expect(payload.config.mqtt_format_pressure).toBe(encodeURIComponent('pressure|\nvalue'))
  })

  it('shows an error when restore is submitted without a file', async () => {
    const wrapper = mountView()
    vi.spyOn(document, 'getElementById').mockReturnValue({
      files: [],
      value: ''
    })

    await wrapper.get('form').trigger('submit')

    expect(mocks.global.messageError).toBe(
      'You need to select one file to restore configuration from'
    )
  })

  it('rejects backup files that are not current PressureMon exports', async () => {
    const wrapper = mountView()
    const fileElement = {
      files: [new File(['backup'], 'backup.txt')],
      value: 'backup.txt'
    }

    vi.spyOn(document, 'getElementById').mockReturnValue(fileElement)
    installFileReader(JSON.stringify({ meta: { software: 'GravityMon', version: '2.2.0' } }))

    await wrapper.get('form').trigger('submit')
    await flushPromises()

    expect(mocks.global.messageError).toBe('Unknown format, unable to process')
    expect(fileElement.value).toBe('')
  })

  it('reports parse errors for invalid json backups', async () => {
    const wrapper = mountView()
    const fileElement = {
      files: [new File(['backup'], 'backup.txt')],
      value: 'backup.txt'
    }

    vi.spyOn(document, 'getElementById').mockReturnValue(fileElement)
    installFileReader('not valid json {{{')

    await wrapper.get('form').trigger('submit')
    await flushPromises()

    expect(mocks.logError).toHaveBeenCalled()
    expect(mocks.global.messageError).toBe('Unable to parse configuration file for PressureMon.')
    expect(fileElement.value).toBe('')
  })

  it('restores and decodes pressure format fields before saving', async () => {
    const wrapper = mountView()
    const fileElement = {
      files: [new File(['backup'], 'backup.txt')],
      value: 'backup.txt'
    }
    const payload = {
      meta: { software: 'PressureMon', version: '0.5.0' },
      config: {
        mdns: 'restored-device',
        http_post_format_pressure: encodeURIComponent('{pressure}'),
        http_post2_format_pressure: encodeURIComponent('{pressure1}'),
        http_get_format_pressure: encodeURIComponent('pressure={pressure}'),
        influxdb2_format_pressure: encodeURIComponent('pressure value={pressure}'),
        mqtt_format_pressure: encodeURIComponent('pressure|value')
      }
    }

    vi.spyOn(document, 'getElementById').mockReturnValue(fileElement)
    installFileReader(JSON.stringify(payload))

    await wrapper.get('form').trigger('submit')
    await flushPromises()

    expect(mocks.config.mdns).toBe('restored-device')
    expect(mocks.config.http_post_format_pressure).toBe('{pressure}')
    expect(mocks.config.http_post2_format_pressure).toBe('{pressure1}')
    expect(mocks.config.http_get_format_pressure).toBe('pressure={pressure}')
    expect(mocks.config.influxdb2_format_pressure).toBe('pressure value={pressure}')
    expect(mocks.config.mqtt_format_pressure).toBe('pressure|value')
    expect(mocks.getConfigChanges).toHaveBeenCalled()
    expect(mocks.config.saveAll).toHaveBeenCalled()
    expect(mocks.global.messageSuccess).toBe('Configuration has been saved to device')
    expect(mocks.global.disabled).toBe(false)
    expect(fileElement.value).toBe('')
  })
})
