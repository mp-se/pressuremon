import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import { mount } from '@vue/test-utils'

const reloadMock = vi.fn()
Object.defineProperty(window, 'location', {
  value: { reload: reloadMock },
  writable: true
})

const mocks = vi.hoisted(() => ({
  global: {
    disabled: false,
    configChanged: false,
    fetchTimeout: 5000,
    token: 'token-1',
    baseURL: '/',
    messageSuccess: '',
    messageError: '',
    clearMessages: vi.fn()
  },
  config: {
    mdns: 'pressuremon-test',
    temp_unit: 'C',
    pressure_unit: 'PSI',
    dark_mode: false,
    saveAll: vi.fn(),
    restart: vi.fn()
  },
  validateCurrentForm: vi.fn(() => true),
  managedFetch: vi.fn(),
  createTimeout: vi.fn(),
  badge: {
    deviceMdnsBadge: vi.fn(() => 0)
  },
  logError: vi.fn(),
  logInfo: vi.fn()
}))

vi.mock('@/modules/pinia', () => ({
  global: mocks.global,
  config: mocks.config
}))

vi.mock('@/modules/badge', () => ({
  __v_isRef: false,
  deviceMdnsBadge: mocks.badge.deviceMdnsBadge
}))

vi.mock('@mp-se/espframework-ui-components', () => ({
  validateCurrentForm: mocks.validateCurrentForm,
  logError: mocks.logError,
  logInfo: mocks.logInfo,
  useFetch: () => ({ managedFetch: mocks.managedFetch }),
  useTimers: () => ({ createTimeout: mocks.createTimeout }),
  BsMessage: { template: '<div><slot /></div>' },
  BsInputText: {
    props: ['modelValue', 'disabled'],
    emits: ['update:modelValue'],
    template:
      '<input class="mdns-input" :value="modelValue" @input="$emit(\'update:modelValue\', $event.target.value)" />'
  },
  BsInputRadio: {
    props: ['modelValue', 'options', 'label'],
    emits: ['update:modelValue'],
    template:
      '<div :data-label="label"><input v-for="option in options" :key="option.value" type="radio" :checked="modelValue===option.value" @change="$emit(\'update:modelValue\', option.value)" /></div>'
  }
}))

import DeviceSettingsView from '../DeviceSettingsView.vue'

function mountView() {
  return mount(DeviceSettingsView, {
    global: {
      stubs: {
        BsMessage: { template: '<div><slot /></div>' },
        BsInputText: {
          props: ['modelValue', 'disabled'],
          emits: ['update:modelValue'],
          template:
            '<input class="mdns-input" :value="modelValue" @input="$emit(\'update:modelValue\', $event.target.value)" />'
        },
        BsInputRadio: {
          props: ['modelValue', 'options', 'label'],
          emits: ['update:modelValue'],
          template:
            '<div :data-label="label"><input v-for="option in options" :key="option.value" type="radio" :checked="modelValue===option.value" @change="$emit(\'update:modelValue\', option.value)" /></div>'
        }
      }
    }
  })
}

describe('DeviceSettingsView', () => {
  beforeEach(() => {
    mocks.global.disabled = false
    mocks.global.configChanged = false
    mocks.global.messageSuccess = ''
    mocks.global.messageError = ''
    mocks.config.mdns = 'pressuremon-test'
    mocks.config.temp_unit = 'C'
    mocks.config.pressure_unit = 'PSI'
    mocks.config.dark_mode = false
    mocks.global.clearMessages.mockReset()
    mocks.config.saveAll.mockReset()
    mocks.config.restart.mockReset()
    mocks.validateCurrentForm.mockReset()
    mocks.validateCurrentForm.mockReturnValue(true)
    mocks.managedFetch.mockReset()
    mocks.createTimeout.mockReset()
    mocks.createTimeout.mockImplementation(() => {})
    mocks.badge.deviceMdnsBadge.mockReset()
    mocks.badge.deviceMdnsBadge.mockReturnValue(0)
    mocks.logError.mockReset()
    mocks.logInfo.mockReset()
    reloadMock.mockReset()
  })

  afterEach(() => {
    vi.restoreAllMocks()
  })

  it('renders warning when mdns is empty and disables save without config changes', () => {
    mocks.config.mdns = ''
    const wrapper = mountView()

    expect(wrapper.text()).toContain('You need to define a mdns name for the device')
    expect(wrapper.get('button[type="submit"]').attributes('disabled')).toBeDefined()
  })

  it('updates settings through bound inputs', async () => {
    const wrapper = mountView()
    await wrapper.get('.mdns-input').setValue('new-mdns')

    const radioGroups = wrapper.findAll('[data-label]')
    await radioGroups[0].findAll('input')[1].trigger('change')
    await radioGroups[1].findAll('input')[2].trigger('change')
    await radioGroups[2].findAll('input')[1].trigger('change')

    expect(mocks.config.mdns).toBe('new-mdns')
    expect(mocks.config.temp_unit).toBe('F')
    expect(mocks.config.pressure_unit).toBe('Bar')
    expect(mocks.config.dark_mode).toBe(true)
  })

  it('saves settings when form validation passes', async () => {
    mocks.global.configChanged = true
    const wrapper = mountView()

    await wrapper.get('form').trigger('submit')

    expect(mocks.validateCurrentForm).toHaveBeenCalled()
    expect(mocks.config.saveAll).toHaveBeenCalled()
  })

  it('does not save settings when form validation fails', async () => {
    mocks.validateCurrentForm.mockReturnValue(false)
    mocks.global.configChanged = true
    const wrapper = mountView()

    await wrapper.get('form').trigger('submit')

    expect(mocks.config.saveAll).not.toHaveBeenCalled()
  })

  it('restart button delegates to config.restart', async () => {
    const wrapper = mountView()
    const restartButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Restart device'))

    await restartButton.trigger('click')

    expect(mocks.config.restart).toHaveBeenCalled()
  })

  it('factory restore success shows message and schedules reload', async () => {
    mocks.managedFetch.mockResolvedValueOnce({
      json: async () => ({ success: true, message: 'Factory restore started' })
    })
    mocks.createTimeout.mockImplementation((callback) => callback())

    const wrapper = mountView()
    const factoryButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Restore factory defaults'))
    await factoryButton.trigger('click')

    expect(mocks.global.clearMessages).toHaveBeenCalled()
    expect(mocks.managedFetch).toHaveBeenCalledWith('/api/factory', expect.any(Object))
    expect(mocks.global.messageSuccess).toBe('Factory restore started')
    expect(mocks.createTimeout).toHaveBeenCalled()
    expect(reloadMock).toHaveBeenCalledWith(true)
  })

  it('factory restore handles unsuccessful response', async () => {
    mocks.managedFetch.mockResolvedValueOnce({
      json: async () => ({ success: false, message: 'Could not restore' })
    })

    const wrapper = mountView()
    const factoryButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Restore factory defaults'))
    await factoryButton.trigger('click')

    expect(mocks.global.messageError).toBe('Could not restore')
    expect(mocks.global.disabled).toBe(false)
  })

  it('factory restore handles fetch errors', async () => {
    mocks.managedFetch.mockRejectedValueOnce(new Error('network'))

    const wrapper = mountView()
    const factoryButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Restore factory defaults'))
    await factoryButton.trigger('click')

    expect(mocks.logError).toHaveBeenCalled()
    expect(mocks.global.messageError).toBe('Failed to do factory restore')
    expect(mocks.global.disabled).toBe(false)
  })
})
