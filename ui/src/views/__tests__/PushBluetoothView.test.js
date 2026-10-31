import { mount } from '@vue/test-utils'
import PushBluetoothView from '../PushBluetoothView.vue'
import { createTestingPinia } from '../../tests/testUtils'

describe('PushBluetoothView (interaction tests)', () => {
  it('mounts without error', () => {
    const pinia = createTestingPinia()
    const wrapper = mount(PushBluetoothView, {
      global: {
        plugins: [pinia],
        stubs: { BsInputText: true, BsInputSwitch: true, BsProgress: true }
      }
    })
    expect(wrapper.exists()).toBe(true)
  })

  it('displays page heading', () => {
    const pinia = createTestingPinia()
    const wrapper = mount(PushBluetoothView, {
      global: {
        plugins: [pinia],
        stubs: { BsInputText: true, BsInputSwitch: true, BsProgress: true }
      }
    })
    expect(wrapper.text()).toContain('Bluetooth')
  })

  it('displays form', () => {
    const pinia = createTestingPinia()
    const wrapper = mount(PushBluetoothView, {
      global: {
        plugins: [pinia],
        stubs: { BsInputText: true, BsInputSwitch: true, BsProgress: true }
      }
    })
    expect(wrapper.find('form').exists()).toBe(true)
  })

  it('has save button', () => {
    const pinia = createTestingPinia()
    const wrapper = mount(PushBluetoothView, {
      global: {
        plugins: [pinia],
        stubs: { BsInputText: true, BsInputSwitch: true, BsProgress: true }
      }
    })
    const buttons = wrapper.findAll('button')
    const saveButton = buttons.find((b) => b.text().includes('Save'))
    expect(saveButton).toBeDefined()
  })

  it('has save function defined', () => {
    const pinia = createTestingPinia()
    const wrapper = mount(PushBluetoothView, {
      global: {
        plugins: [pinia],
        stubs: { BsInputText: true, BsInputSwitch: true, BsProgress: true }
      }
    })
    expect(typeof wrapper.vm.save).toBe('function')
  })

  it('has config state defined', () => {
    const pinia = createTestingPinia()
    const wrapper = mount(PushBluetoothView, {
      global: {
        plugins: [pinia],
        stubs: { BsInputText: true, BsInputSwitch: true, BsProgress: true }
      }
    })
    expect(wrapper.vm.config).toBeDefined()
  })

  it('displays container layout', () => {
    const pinia = createTestingPinia()
    const wrapper = mount(PushBluetoothView, {
      global: {
        plugins: [pinia],
        stubs: { BsInputText: true, BsInputSwitch: true, BsProgress: true }
      }
    })
    expect(wrapper.find('.container').exists()).toBe(true)
  })
})
describe('PushBluetoothView (pressure action tests)', () => {
  const mountView = () =>
    mount(PushBluetoothView, {
      global: {
        plugins: [createTestingPinia()],
        stubs: { BsInputRadio: true }
      }
    })

  beforeEach(() => vi.clearAllMocks())

  it('saves bluetooth settings when valid', async () => {
    const { config, global } = await import('@/modules/pinia')
    const wrapper = mountView()
    await wrapper.vm.save()
    expect(global.clearMessages).toHaveBeenCalled()
    expect(config.saveAll).toHaveBeenCalled()
  })

  it('does not save bluetooth settings when validation fails', async () => {
    const { validateCurrentForm } = await import('@mp-se/espframework-ui-components')
    const { config } = await import('@/modules/pinia')
    vi.mocked(validateCurrentForm).mockReturnValue(false)
    config.saveAll = vi.fn()
    const wrapper = mountView()
    await wrapper.vm.save()
    expect(config.saveAll).not.toHaveBeenCalled()
  })

  it('shows unavailable message when bluetooth is unsupported', async () => {
    const { global } = await import('@/modules/pinia')
    global.feature.ble = false
    const wrapper = mountView()
    expect(wrapper.text()).toContain('Bluetooth is not available on this platform')
    global.feature.ble = true
  })

  it('updates the bluetooth format model from child emits', async () => {
    const { config, global } = await import('@/modules/pinia')
    global.feature.ble = true

    const wrapper = mount(PushBluetoothView, {
      global: {
        plugins: [createTestingPinia()],
        stubs: {
          BsInputRadio: {
            props: ['label'],
            emits: ['update:modelValue'],
            template:
              '<button class="radio-emit" :data-label="label" @click="$emit(\'update:modelValue\', 1)">{{ label }}</button>'
          }
        }
      }
    })

    await wrapper.get('.radio-emit').trigger('click')
    expect(config.ble_format).toBe(1)
  })
})
