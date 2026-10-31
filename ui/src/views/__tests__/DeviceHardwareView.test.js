import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import { mount } from '@vue/test-utils'

const mocks = vi.hoisted(() => ({
  global: {
    disabled: false,
    configChanged: false,
    clearMessages: vi.fn()
  },
  config: {
    voltage_factor: 1.2,
    voltage_config: 4.2,
    battery_saving: false,
    battery_type: 0,
    saveAll: vi.fn(),
    restart: vi.fn()
  },
  status: {
    battery: 3.7
  },
  validateCurrentForm: vi.fn(() => true)
}))

vi.mock('@/modules/pinia', () => ({
  global: mocks.global,
  config: mocks.config,
  status: mocks.status
}))

vi.mock('@mp-se/espframework-ui-components', () => ({
  validateCurrentForm: mocks.validateCurrentForm,
  BsInputNumber: {
    props: ['modelValue', 'label', 'unit', 'disabled'],
    template:
      '<div class="number-field" :data-label="label" :data-model-value="String(modelValue)" :data-unit="unit" :data-disabled="String(disabled)">{{ label }} {{ modelValue }} {{ unit }}</div>'
  },
  BsInputSwitch: {
    props: ['modelValue', 'label', 'disabled'],
    template:
      '<div class="switch-field" :data-label="label" :data-model-value="String(modelValue)" :data-disabled="String(disabled)">{{ label }} {{ modelValue }}</div>'
  },
  BsSelect: {
    props: ['modelValue', 'label', 'disabled'],
    template:
      '<div class="select-field" :data-label="label" :data-model-value="String(modelValue)" :data-disabled="String(disabled)">{{ label }} {{ modelValue }}</div>'
  }
}))

import DeviceHardwareView from '../DeviceHardwareView.vue'

function mountView() {
  return mount(DeviceHardwareView, {
    global: {
      stubs: {
        BsInputNumber: {
          props: ['modelValue', 'label', 'unit', 'disabled'],
          template:
            '<div class="number-field" :data-label="label" :data-model-value="String(modelValue)" :data-unit="unit" :data-disabled="String(disabled)">{{ label }} {{ modelValue }} {{ unit }}</div>'
        },
        BsInputSwitch: {
          props: ['modelValue', 'label', 'disabled'],
          template:
            '<div class="switch-field" :data-label="label" :data-model-value="String(modelValue)" :data-disabled="String(disabled)">{{ label }} {{ modelValue }}</div>'
        },
        BsSelect: {
          props: ['modelValue', 'label', 'disabled'],
          template:
            '<div class="select-field" :data-label="label" :data-model-value="String(modelValue)" :data-disabled="String(disabled)">{{ label }} {{ modelValue }}</div>'
        }
      }
    }
  })
}

describe('DeviceHardwareView', () => {
  beforeEach(() => {
    mocks.global.disabled = false
    mocks.global.configChanged = false
    mocks.global.clearMessages.mockReset()
    mocks.config.saveAll.mockReset()
    mocks.config.restart.mockReset()
    mocks.validateCurrentForm.mockReset()
    mocks.validateCurrentForm.mockReturnValue(true)
    mocks.status.battery = 3.7
  })

  afterEach(() => {
    vi.restoreAllMocks()
  })

  it('renders the hardware form with live battery voltage', () => {
    const wrapper = mountView()

    expect(wrapper.text()).toContain('Device - Hardware')
    expect(wrapper.text()).toContain('3.7 V')
    expect(wrapper.get('button[type="submit"]').attributes('disabled')).toBeDefined()
  })

  it('enables save when configChanged is true', () => {
    mocks.global.configChanged = true
    const wrapper = mountView()

    expect(wrapper.get('button[type="submit"]').attributes('disabled')).toBeUndefined()
  })

  it('saves hardware settings when form validation passes', async () => {
    mocks.global.configChanged = true
    const wrapper = mountView()

    await wrapper.get('form').trigger('submit')

    expect(mocks.validateCurrentForm).toHaveBeenCalled()
    expect(mocks.global.clearMessages).toHaveBeenCalled()
    expect(mocks.config.saveAll).toHaveBeenCalled()
  })

  it('does not save when form validation fails', async () => {
    mocks.validateCurrentForm.mockReturnValue(false)
    mocks.global.configChanged = true
    const wrapper = mountView()

    await wrapper.get('form').trigger('submit')

    expect(mocks.global.clearMessages).not.toHaveBeenCalled()
    expect(mocks.config.saveAll).not.toHaveBeenCalled()
  })

  it('restart button calls config.restart', async () => {
    const wrapper = mountView()
    const restartButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Restart device'))

    await restartButton.trigger('click')

    expect(mocks.config.restart).toHaveBeenCalled()
  })

  it('binds the current hardware values into the child field props', () => {
    mocks.config.voltage_factor = 1.45
    mocks.config.voltage_config = 4.35
    mocks.config.battery_saving = true
    mocks.config.battery_type = 1

    const wrapper = mountView()

    expect(wrapper.find('[data-label="Voltage factor"]').attributes('data-model-value')).toBe(
      '1.45'
    )
    expect(wrapper.find('[data-label="Voltage factor"]').attributes('data-unit')).toBe('3.7 V')
    expect(wrapper.find('[data-label="Voltage config"]').attributes('data-model-value')).toBe(
      '4.35'
    )
    expect(wrapper.find('[data-label="Battery saving"]').attributes('data-model-value')).toBe(
      'true'
    )
    expect(wrapper.find('[data-label="Battery type"]').attributes('data-model-value')).toBe('1')
  })

  it('passes disabled state through to hardware field controls', () => {
    mocks.global.disabled = true
    const wrapper = mountView()

    expect(wrapper.find('[data-label="Voltage factor"]').attributes('data-disabled')).toBe('true')
    expect(wrapper.find('[data-label="Battery saving"]').attributes('data-disabled')).toBe('true')
    expect(wrapper.find('[data-label="Battery type"]').attributes('data-disabled')).toBe('true')
    expect(wrapper.findAll('button').at(1).attributes('disabled')).toBeDefined()
  })

  it('updates hardware models from child component emits', async () => {
    const wrapper = mount(DeviceHardwareView, {
      global: {
        stubs: {
          BsInputNumber: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="number-emit" :data-label="label" @click="$emit(\'update:modelValue\', 2.5)">{{ label }}</button>'
          },
          BsInputSwitch: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="switch-emit" :data-label="label" @click="$emit(\'update:modelValue\', true)">{{ label }}</button>'
          },
          BsSelect: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="select-emit" :data-label="label" @click="$emit(\'update:modelValue\', 1)">{{ label }}</button>'
          }
        }
      }
    })

    for (const button of wrapper.findAll('.number-emit, .switch-emit, .select-emit')) {
      await button.trigger('click')
    }

    expect(mocks.config.voltage_factor).toBe(2.5)
    expect(mocks.config.voltage_config).toBe(2.5)
    expect(mocks.config.battery_saving).toBe(true)
    expect(mocks.config.battery_type).toBe(1)
  })
})
