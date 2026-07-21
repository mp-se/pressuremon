import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import { mount } from '@vue/test-utils'

const mocks = vi.hoisted(() => ({
  global: {
    disabled: false,
    configChanged: false,
    platform: 'ESP32S3'
  },
  config: {
    token: '',
    token2: '',
    sleep_interval: 600,
    push_timeout: 15,
    wifi_direct_ssid: '',
    wifi_direct_pass: '',
    use_wifi_direct: false,
    ble_format: 0,
    http_post_target: 'https://example.test/post',
    http_post2_target: '',
    http_get_target: '',
    influxdb2_target: '',
    mqtt_target: '',
    saveAll: vi.fn()
  },
  validateCurrentForm: vi.fn(() => true),
  logDebug: vi.fn(),
  logError: vi.fn()
}))

vi.mock('@/modules/pinia', async () => {
  const vue = await import('vue')
  return {
    global: vue.reactive(mocks.global),
    config: vue.reactive(mocks.config)
  }
})

vi.mock('pinia', async () => {
  const vue = await import('vue')
  return {
    storeToRefs: (store) => ({
      sleep_interval: vue.toRef(store, 'sleep_interval')
    })
  }
})

vi.mock('@mp-se/espframework-ui-components', () => ({
  validateCurrentForm: mocks.validateCurrentForm,
  logDebug: mocks.logDebug,
  logError: mocks.logError,
  BsMessage: { template: '<div><slot /></div>' },
  BsInputText: {
    props: ['label', 'modelValue'],
    template: '<div>{{ label }} {{ modelValue }}</div>'
  },
  BsInputNumber: {
    props: ['label', 'modelValue'],
    template: '<div>{{ label }} {{ modelValue }}</div>'
  },
  BsInputReadonly: {
    props: ['modelValue', 'label'],
    template:
      '<div><span>{{ label }}</span><span class="battery-life">{{ modelValue }}</span></div>'
  },
  BsInputSwitch: {
    props: ['label', 'modelValue'],
    template: '<div>{{ label }} {{ modelValue }}</div>'
  }
}))

import PushSettingsView from '../PushSettingsView.vue'

function mountView() {
  return mount(PushSettingsView, {
    global: {
      stubs: {
        BsMessage: { template: '<div><slot /></div>' },
        BsInputText: {
          props: ['label', 'modelValue'],
          template: '<div>{{ label }} {{ modelValue }}</div>'
        },
        BsInputNumber: {
          props: ['label', 'modelValue'],
          template: '<div>{{ label }}{{ modelValue }}</div>'
        },
        BsInputReadonly: {
          props: ['modelValue', 'label'],
          template:
            '<div><span>{{ label }}</span><span class="battery-life">{{ modelValue }}</span></div>'
        },
        BsInputSwitch: {
          props: ['label', 'modelValue'],
          template: '<div>{{ label }} {{ modelValue }}</div>'
        }
      }
    }
  })
}

describe('PushSettingsView', () => {
  beforeEach(() => {
    mocks.global.disabled = false
    mocks.global.configChanged = false
    mocks.global.platform = 'ESP32S3'
    mocks.config.token = ''
    mocks.config.token2 = ''
    mocks.config.sleep_interval = 600
    mocks.config.push_timeout = 15
    mocks.config.wifi_direct_ssid = ''
    mocks.config.wifi_direct_pass = ''
    mocks.config.use_wifi_direct = false
    mocks.config.ble_format = 0
    mocks.config.http_post_target = 'https://example.test/post'
    mocks.config.http_post2_target = ''
    mocks.config.http_get_target = ''
    mocks.config.influxdb2_target = ''
    mocks.config.mqtt_target = ''
    mocks.config.saveAll.mockReset()
    mocks.validateCurrentForm.mockReset()
    mocks.validateCurrentForm.mockReturnValue(true)
    mocks.logDebug.mockReset()
    mocks.logError.mockReset()
  })

  afterEach(() => {
    vi.restoreAllMocks()
  })

  it('renders push settings with the computed interval input and battery section', () => {
    const wrapper = mountView()

    expect(wrapper.text()).toContain('Push - Settings')
    expect(wrapper.text()).toContain('Sleep interval600')
    expect(wrapper.text()).toContain('Estimated battery life')
  })

  it('shows low sleep interval warning', () => {
    mocks.config.sleep_interval = 120
    const wrapper = mountView()

    expect(wrapper.text()).toContain('A sleep-interval of less than 300 will reduce battery life')
  })

  it('renders the low interval branch with updated sleep value', () => {
    mocks.config.sleep_interval = 125
    const wrapper = mountView()

    expect(wrapper.text()).toContain('A sleep-interval of less than 300 will reduce battery life')
    expect(wrapper.text()).toContain('125')
  })

  it('logs and leaves battery estimate empty when no push target is configured', () => {
    mocks.config.http_post_target = ''
    mocks.config.http_post2_target = ''
    mocks.config.http_get_target = ''
    mocks.config.influxdb2_target = ''
    mocks.config.mqtt_target = ''
    mocks.config.ble_format = 0

    const wrapper = mountView()

    expect(mocks.logError).toHaveBeenCalled()
    expect(wrapper.get('.battery-life').text()).toBe('')
  })

  it('calculates battery life for wifi mode on ESP32C3', () => {
    mocks.global.platform = 'ESP32C3'
    mocks.config.http_post_target = 'https://example.test/post'
    mocks.config.ble_format = 0

    mountView()

    expect(mocks.logDebug).toHaveBeenCalled()
  })

  it('logs an error for unknown wifi platforms', () => {
    mocks.global.platform = 'UNKNOWN'
    mountView()
    expect(mocks.logError).toHaveBeenCalled()
  })

  it('calculates battery life for wifi mode on ESP32S2', () => {
    mocks.global.platform = 'ESP32S2'
    mountView()
    expect(mocks.logDebug).toHaveBeenCalled()
  })

  it('calculates battery life for wifi mode on ESP32S3', () => {
    mocks.global.platform = 'ESP32S3'
    mountView()
    expect(mocks.logDebug).toHaveBeenCalled()
  })

  it('calculates battery life for ble-only mode on ESP32S3', () => {
    mocks.global.platform = 'ESP32S3'
    mocks.config.http_post_target = ''
    mocks.config.ble_format = 1
    mountView()
    expect(mocks.logError).not.toHaveBeenCalled()
  })

  it('renders push settings form', () => {
    const wrapper = mountView()
    expect(wrapper.text()).toContain('Push - Settings')
    expect(wrapper.text()).toContain('Sleep interval')
  })

  it('updates all push settings models from child component emits', async () => {
    const wrapper = mount(PushSettingsView, {
      global: {
        stubs: {
          BsMessage: { template: '<div><slot /></div>' },
          BsInputText: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="text-emit" :data-label="label" @click="$emit(\'update:modelValue\', label + \' updated\')">{{ label }}</button>'
          },
          BsInputNumber: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="number-emit" :data-label="label" @click="$emit(\'update:modelValue\', 321)">{{ label }}</button>'
          },
          BsInputReadonly: {
            props: ['modelValue', 'label'],
            template:
              '<div><span>{{ label }}</span><span class="battery-life">{{ modelValue }}</span></div>'
          },
          BsInputSwitch: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="switch-emit" :data-label="label" @click="$emit(\'update:modelValue\', true)">{{ label }}</button>'
          }
        }
      }
    })

    for (const button of wrapper.findAll('.text-emit, .number-emit, .switch-emit')) {
      await button.trigger('click')
    }

    expect(mocks.config.token).toBe('Token 1 updated')
    expect(mocks.config.token2).toBe('Token 2 updated')
    expect(mocks.config.sleep_interval).toBe(321)
    expect(mocks.config.push_timeout).toBe(321)
  })

  it('updates the sleep interval label when the interval changes after mount', async () => {
    const wrapper = mountView()

    expect(wrapper.text()).toContain('Sleep interval600')

    mocks.config.sleep_interval = 905
    wrapper.vm.createSleepLabel()
    await wrapper.vm.$nextTick()

    expect(wrapper.text()).toContain('Sleep interval(15 min 5 sec)905')
  })

  it('recomputes battery life when the interval changes after mount', async () => {
    const wrapper = mountView()
    const originalValue = wrapper.get('.battery-life').text()

    mocks.config.sleep_interval = 1200
    await wrapper.vm.$nextTick()
    await wrapper.vm.$nextTick()

    expect(wrapper.get('.battery-life').text()).not.toBe('')
    expect(wrapper.get('.battery-life').text()).not.toBe(originalValue)
  })

  it('renders the computed sleep interval label on initial mount', async () => {
    mocks.config.sleep_interval = 301
    const wrapper = mountView()
    wrapper.vm.createSleepLabel()
    await wrapper.vm.$nextTick()

    expect(wrapper.text()).toContain('Sleep interval(5 min 1 sec)301')
  })

  it('calculates battery life for ble-only mode on ESP32C3', () => {
    mocks.global.platform = 'ESP32C3'
    mocks.config.http_post_target = ''
    mocks.config.ble_format = 1

    mountView()

    expect(mocks.logError).not.toHaveBeenCalled()
    expect(mocks.logDebug).toHaveBeenCalled()
  })

  it('calculates battery life for ble-only mode on ESP32S2', () => {
    mocks.global.platform = 'ESP32S2'
    mocks.config.http_post_target = ''
    mocks.config.ble_format = 1

    mountView()

    expect(mocks.logError).not.toHaveBeenCalled()
    expect(mocks.logDebug).toHaveBeenCalled()
  })

  it('logs an error for unknown ble-only platforms', () => {
    mocks.global.platform = 'UNKNOWN'
    mocks.config.http_post_target = ''
    mocks.config.ble_format = 1

    mountView()

    expect(mocks.logError).toHaveBeenCalled()
    expect(mocks.logDebug).toHaveBeenCalled()
  })

  it('saves push settings when the form is valid', async () => {
    mocks.global.configChanged = true
    const wrapper = mountView()

    await wrapper.get('form').trigger('submit')

    expect(mocks.validateCurrentForm).toHaveBeenCalled()
    expect(mocks.config.saveAll).toHaveBeenCalled()
  })

  it('does not save push settings when the form is invalid', async () => {
    mocks.validateCurrentForm.mockReturnValue(false)
    mocks.global.configChanged = true
    const wrapper = mountView()

    await wrapper.get('form').trigger('submit')

    expect(mocks.config.saveAll).not.toHaveBeenCalled()
  })
})
