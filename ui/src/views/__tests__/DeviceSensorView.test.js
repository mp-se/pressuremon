import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import { mount, flushPromises } from '@vue/test-utils'

const mocks = vi.hoisted(() => ({
  global: {
    disabled: false,
    configChanged: false,
    messageSuccess: '',
    messageError: '',
    clearMessages: vi.fn()
  },
  config: {
    tempsensor_resolution: 12,
    temp_adjustment_value: 0,
    temp_unit: 'C',
    sensor_type: 1,
    sensor1_type: 0,
    pressure_adjustment: 0,
    pressure1_adjustment: 0,
    custom_min_voltage: 0,
    custom_max_voltage: 5,
    custom_min_pressure: 0,
    custom_max_pressure: 100,
    custom1_min_voltage: 0,
    custom1_max_voltage: 5,
    custom1_min_pressure: 0,
    custom1_max_pressure: 100,
    pressure_unit: 'PSI',
    saveAll: vi.fn(),
    restart: vi.fn(),
    load: vi.fn()
  },
  status: {
    max_sensors: 1,
    adc_found: false,
    self_check: {
      sensor_connected: true
    }
  },
  validateCurrentForm: vi.fn(() => true),
  badge: {
    deviceSensorCalibratedBadge: vi.fn(() => 0)
  },
  http: {
    request: vi.fn(),
    getJson: vi.fn()
  },
  createTimeout: vi.fn((callback) => callback()),
  logDebug: vi.fn(),
  logError: vi.fn(),
  logInfo: vi.fn()
}))

vi.mock('@/modules/pinia', () => ({
  global: mocks.global,
  config: mocks.config,
  status: mocks.status
}))

vi.mock('@/modules/badge', () => ({
  __v_isRef: false,
  deviceSensorCalibratedBadge: mocks.badge.deviceSensorCalibratedBadge
}))

vi.mock('@mp-se/espframework-ui-components', () => ({
  validateCurrentForm: mocks.validateCurrentForm,
  logDebug: mocks.logDebug,
  logError: mocks.logError,
  logInfo: mocks.logInfo,
  useTimers: () => ({ createTimeout: mocks.createTimeout }),
  sharedHttpClient: mocks.http,
  BsInputRadio: {
    props: ['label', 'modelValue', 'disabled'],
    template:
      '<div class="radio-field" :data-label="label" :data-model-value="String(modelValue)" :data-disabled="String(disabled)">{{ label }}</div>'
  },
  BsInputNumber: {
    props: ['label', 'modelValue', 'disabled', 'unit'],
    template:
      '<div class="number-field" :data-label="label" :data-model-value="String(modelValue)" :data-disabled="String(disabled)" :data-unit="unit">{{ label }}</div>'
  },
  BsSelect: {
    props: ['label', 'modelValue', 'disabled'],
    template:
      '<div class="select-field" :data-label="label" :data-model-value="String(modelValue)" :data-disabled="String(disabled)">{{ label }}</div>'
  }
}))

import DeviceSensorView from '../DeviceSensorView.vue'

function mountView() {
  return mount(DeviceSensorView, {
    global: {
      stubs: {
        BsInputRadio: {
          props: ['label', 'modelValue', 'disabled'],
          template:
            '<div class="radio-field" :data-label="label" :data-model-value="String(modelValue)" :data-disabled="String(disabled)">{{ label }}</div>'
        },
        BsInputNumber: {
          props: ['label', 'modelValue', 'disabled', 'unit'],
          template:
            '<div class="number-field" :data-label="label" :data-model-value="String(modelValue)" :data-disabled="String(disabled)" :data-unit="unit">{{ label }}</div>'
        },
        BsSelect: {
          props: ['label', 'modelValue', 'disabled'],
          template:
            '<div class="select-field" :data-label="label" :data-model-value="String(modelValue)" :data-disabled="String(disabled)">{{ label }}</div>'
        }
      }
    }
  })
}

describe('DeviceSensorView', () => {
  beforeEach(() => {
    mocks.global.disabled = false
    mocks.global.configChanged = false
    mocks.global.messageSuccess = ''
    mocks.global.messageError = ''
    mocks.global.clearMessages.mockReset()
    mocks.config.sensor_type = 1
    mocks.config.sensor1_type = 0
    mocks.config.saveAll.mockReset()
    mocks.config.restart.mockReset()
    mocks.config.load.mockReset()
    mocks.config.load.mockResolvedValue(true)
    mocks.status.max_sensors = 1
    mocks.status.adc_found = false
    mocks.status.self_check.sensor_connected = true
    mocks.validateCurrentForm.mockReset()
    mocks.validateCurrentForm.mockReturnValue(true)
    mocks.badge.deviceSensorCalibratedBadge.mockReset()
    mocks.badge.deviceSensorCalibratedBadge.mockReturnValue(0)
    mocks.http.request.mockReset()
    mocks.http.getJson.mockReset()
    mocks.createTimeout.mockClear()
    mocks.logDebug.mockReset()
    mocks.logError.mockReset()
    mocks.logInfo.mockReset()
  })

  afterEach(() => {
    vi.restoreAllMocks()
  })

  it('renders the sensor form and disables save without config changes', () => {
    const wrapper = mountView()

    expect(wrapper.text()).toContain('Device - Hardware')
    expect(wrapper.get('button[type="submit"]').attributes('disabled')).toBeDefined()
  })

  it('adds the custom analog sensor option when adc support is present', async () => {
    mocks.status.adc_found = true
    mountView()
    await flushPromises()

    expect(mocks.logDebug).toHaveBeenCalled()
  })

  it('renders secondary and analog sensor fields when dual custom sensors are enabled', () => {
    mocks.status.adc_found = true
    mocks.status.max_sensors = 2
    mocks.config.sensor_type = 1000
    mocks.config.sensor1_type = 1000

    const wrapper = mountView()

    expect(wrapper.text()).toContain('Pressure Sensor 2')
    expect(wrapper.text()).toContain('Analog - Min Voltage')
    expect(wrapper.text()).toContain('Analog - Max Pressure 2')
  })

  it('disables calibration when the sensor is not connected', () => {
    mocks.status.self_check.sensor_connected = false
    const wrapper = mountView()
    const calibrateButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Calibrate pressure'))
    expect(calibrateButton.attributes('disabled')).toBeDefined()
  })

  it('shows the calibration badge when calibration is required', () => {
    mocks.badge.deviceSensorCalibratedBadge.mockReturnValueOnce(1)
    const wrapper = mountView()
    expect(wrapper.text()).toContain('1')
  })

  it('renders secondary pressure adjustment when a second sensor is enabled', () => {
    mocks.status.max_sensors = 2
    mocks.config.sensor1_type = 1
    const wrapper = mountView()
    expect(wrapper.text()).toContain('Pressure adjustment 2')
  })

  it('binds the sensor values into child field props', () => {
    mocks.config.tempsensor_resolution = 11
    mocks.config.temp_adjustment_value = 1.25
    mocks.config.sensor_type = 4
    mocks.config.pressure_adjustment = 0.75

    const wrapper = mountView()

    expect(wrapper.find('[data-label="DS18B20 resolution"]').attributes('data-model-value')).toBe(
      '11'
    )
    expect(
      wrapper.find('[data-label="Temperature sensor adjustment"]').attributes('data-model-value')
    ).toBe('1.25')
    expect(wrapper.find('[data-label="Pressure Sensor"]').attributes('data-model-value')).toBe('4')
    expect(wrapper.find('[data-label="Pressure adjustment"]').attributes('data-model-value')).toBe(
      '0.75'
    )
    expect(wrapper.find('[data-label="Pressure adjustment"]').attributes('data-unit')).toBe('PSI')
  })

  it('disables pressure adjustment fields when the sensors are unset', () => {
    mocks.status.max_sensors = 2
    mocks.config.sensor_type = 0
    mocks.config.sensor1_type = 0

    const wrapper = mountView()

    expect(wrapper.find('[data-label="Pressure adjustment"]').attributes('data-disabled')).toBe(
      'true'
    )
    expect(wrapper.find('[data-label="Pressure adjustment 2"]').attributes('data-disabled')).toBe(
      'true'
    )
  })

  it('passes the global disabled state through to sensor fields', () => {
    mocks.global.disabled = true
    const wrapper = mountView()

    expect(wrapper.find('[data-label="DS18B20 resolution"]').attributes('data-disabled')).toBe(
      'true'
    )
    expect(wrapper.find('[data-label="Pressure Sensor"]').attributes('data-disabled')).toBe('true')
  })

  it('updates sensor models from child component emits', async () => {
    mocks.status.max_sensors = 2
    mocks.config.sensor_type = 1
    mocks.config.sensor1_type = 1

    const wrapper = mount(DeviceSensorView, {
      global: {
        stubs: {
          BsInputRadio: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="radio-emit" :data-label="label" @click="$emit(\'update:modelValue\', 10)">{{ label }}</button>'
          },
          BsInputNumber: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="number-emit" :data-label="label" @click="$emit(\'update:modelValue\', 4.5)">{{ label }}</button>'
          },
          BsSelect: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="select-emit" :data-label="label" @click="$emit(\'update:modelValue\', label.includes(\'2\') ? 6 : 5)">{{ label }}</button>'
          }
        }
      }
    })

    for (const button of wrapper.findAll('.radio-emit, .number-emit, .select-emit')) {
      await button.trigger('click')
    }

    expect(mocks.config.tempsensor_resolution).toBe(10)
    expect(mocks.config.temp_adjustment_value).toBe(4.5)
    expect(mocks.config.sensor_type).toBe(5)
    expect(mocks.config.sensor1_type).toBe(6)
    expect(mocks.config.pressure_adjustment).toBe(4.5)
    expect(mocks.config.pressure1_adjustment).toBe(4.5)
  })

  it('updates analog sensor models from conditional child emits', async () => {
    mocks.status.max_sensors = 2
    mocks.status.adc_found = true
    mocks.config.sensor_type = 1000
    mocks.config.sensor1_type = 1000

    const wrapper = mount(DeviceSensorView, {
      global: {
        stubs: {
          BsInputRadio: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="radio-emit" :data-label="label" @click="$emit(\'update:modelValue\', 12)">{{ label }}</button>'
          },
          BsInputNumber: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="number-emit" :data-label="label" @click="$emit(\'update:modelValue\', 7.5)">{{ label }}</button>'
          },
          BsSelect: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="select-emit" :data-label="label" @click="$emit(\'update:modelValue\', 1000)">{{ label }}</button>'
          }
        }
      }
    })

    for (const button of wrapper.findAll('.number-emit, .select-emit, .radio-emit')) {
      await button.trigger('click')
    }

    expect(mocks.config.custom_min_voltage).toBe(7.5)
    expect(mocks.config.custom1_min_voltage).toBe(7.5)
    expect(mocks.config.custom_max_voltage).toBe(7.5)
    expect(mocks.config.custom1_max_voltage).toBe(7.5)
    expect(mocks.config.custom_min_pressure).toBe(7.5)
    expect(mocks.config.custom1_min_pressure).toBe(7.5)
    expect(mocks.config.custom_max_pressure).toBe(7.5)
    expect(mocks.config.custom1_max_pressure).toBe(7.5)
  })

  it('saves sensor settings when form validation passes', async () => {
    mocks.global.configChanged = true
    const wrapper = mountView()

    await wrapper.get('form').trigger('submit')

    expect(mocks.global.clearMessages).toHaveBeenCalled()
    expect(mocks.config.saveAll).toHaveBeenCalled()
  })

  it('does not save sensor settings when form validation fails', async () => {
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

  it('calibrate succeeds after polling and reloads config', async () => {
    mocks.http.request.mockResolvedValueOnce({ ok: true })
    mocks.http.getJson
      .mockResolvedValueOnce({ status: true })
      .mockResolvedValueOnce({ status: false })

    const wrapper = mountView()
    const calibrateButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Calibrate pressure'))
    await calibrateButton.trigger('click')
    await flushPromises()

    expect(mocks.http.request).toHaveBeenCalledWith('api/calibrate', { method: 'GET' })
    expect(mocks.http.getJson).toHaveBeenCalledWith('api/calibrate/status')
    expect(mocks.config.load).toHaveBeenCalled()
    expect(mocks.global.messageSuccess).toBe('Sensor calibrated')
    expect(mocks.global.disabled).toBe(false)
  })

  it('reports an error when calibration start fails', async () => {
    mocks.http.request.mockResolvedValueOnce({
      ok: false,
      status: 500,
      statusText: 'Server Error',
      text: async () => 'boom'
    })

    const wrapper = mountView()
    const calibrateButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Calibrate pressure'))
    await calibrateButton.trigger('click')
    await flushPromises()

    expect(mocks.global.messageError).toBe('Failed to start calibration')
    expect(mocks.global.disabled).toBe(false)
  })

  it('reports an error when calibration status polling fails', async () => {
    mocks.http.request.mockResolvedValueOnce({ ok: true })
    mocks.http.getJson.mockRejectedValueOnce(new Error('poll failed'))

    const wrapper = mountView()
    const calibrateButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Calibrate pressure'))
    await calibrateButton.trigger('click')
    await flushPromises()

    expect(mocks.global.messageError).toBe('Failed to get calibrate status')
    expect(mocks.global.disabled).toBe(false)
  })

  it('reports an unexpected calibration start exception', async () => {
    mocks.http.request.mockRejectedValueOnce(new Error('Calibration failed unexpectedly'))

    const wrapper = mountView()
    const calibrateButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Calibrate pressure'))
    await calibrateButton.trigger('click')
    await flushPromises()

    expect(mocks.global.messageError).toBe('Calibration failed unexpectedly')
    expect(mocks.global.disabled).toBe(false)
  })

  it('reports an error when config reload fails after calibration', async () => {
    mocks.http.request.mockResolvedValueOnce({ ok: true })
    mocks.http.getJson.mockResolvedValueOnce({ status: false })
    mocks.config.load.mockResolvedValueOnce(false)

    const wrapper = mountView()
    const calibrateButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Calibrate pressure'))
    await calibrateButton.trigger('click')
    await flushPromises()

    expect(mocks.global.messageError).toBe('Failed to load configuration after calibration')
    expect(mocks.global.disabled).toBe(false)
  })

  it('surfaces a fallback start failure message when the thrown error has no message', async () => {
    mocks.http.request.mockRejectedValueOnce({})

    const wrapper = mountView()
    const calibrateButton = wrapper
      .findAll('button')
      .find((entry) => entry.text().includes('Calibrate pressure'))
    await calibrateButton.trigger('click')
    await flushPromises()

    expect(mocks.global.messageError).toBe('Calibration failed unexpectedly')
    expect(mocks.global.disabled).toBe(false)
  })
})
