import { describe, it, expect, beforeEach, vi, afterEach } from 'vitest'
import { mount, shallowMount } from '@vue/test-utils'
import HomeView from '../HomeView.vue'
import { createTestingPinia } from '../../tests/testUtils'

describe('HomeView (smoke)', () => {
  it('mounts without error', () => {
    const pinia = createTestingPinia()
    const wrapper = shallowMount(HomeView, { global: { plugins: [pinia] } })
    expect(wrapper.exists()).toBe(true)
  })
})

describe('HomeView (pressure function tests)', () => {
  beforeEach(async () => {
    vi.clearAllMocks()
    const { status, config } = await import('@/modules/pinia')
    status.sleep_mode = false
    status.connected = true
    status.pressure = 10.5
    status.pressure1 = 11.1
    status.temp = 20
    status.battery = 3.7
    status.pressure_unit = 'PSI'
    status.temp_unit = 'C'
    status.rssi = -50
    status.wifi_ssid = 'test-ssid'
    status.ip = '192.168.1.100'
    status.free_heap = 32
    status.total_heap = 64
    status.id = 'device-1'
    status.self_check.push_targets = true
    status.self_check.sensor_configured = true
    status.self_check.sensor_connected = true
    status.self_check.battery_level = true
    status.load = vi.fn(async () => true)
    config.setSleepMode = vi.fn(async () => true)
  })

  it('renders warning messages for missing push targets and sensors', async () => {
    const { status } = await import('@/modules/pinia')
    status.self_check.push_targets = false
    status.self_check.sensor_configured = false
    status.self_check.sensor_connected = false

    const wrapper = mount(HomeView, {
      global: {
        plugins: [createTestingPinia()],
        stubs: {
          BsMessage: { template: '<div><slot /></div>' },
          BsCard: { template: '<div><slot /></div>' }
        }
      }
    })

    expect(wrapper.text()).toContain('No remote services are active')
    expect(wrapper.text()).toContain('No sensor has been configured')
    expect(wrapper.text()).toContain('No sensor is detected')
  })

  it('renders the battery error card when the battery level is invalid', async () => {
    const { status } = await import('@/modules/pinia')
    status.self_check.battery_level = false

    const wrapper = mount(HomeView, {
      global: {
        plugins: [createTestingPinia()],
        stubs: {
          BsMessage: true,
          BsCard: { template: '<div><slot /></div>' }
        }
      }
    })

    expect(wrapper.text()).toContain('Battery level not valid')
  })

  it('initializes flag from status.sleep_mode on mount', async () => {
    const { status } = await import('@/modules/pinia')
    status.sleep_mode = true

    const wrapper = mount(HomeView, {
      global: {
        plugins: [createTestingPinia()],
        stubs: {
          BsMessage: true,
          BsCard: { template: '<div><slot /></div>' }
        }
      }
    })

    expect(wrapper.vm.flag).toBe(true)
  })

  it('calls config.setSleepMode when the force-config switch changes', async () => {
    const { config } = await import('@/modules/pinia')
    const wrapper = mount(HomeView, {
      global: {
        plugins: [createTestingPinia()],
        stubs: {
          BsMessage: true,
          BsCard: { template: '<div><slot /></div>' }
        }
      }
    })

    await wrapper.get('input[type="checkbox"]').setChecked(true)
    expect(config.setSleepMode).toHaveBeenCalledWith(true)
  })

  it('refresh loads status data and unmount covers cleanup path', async () => {
    const { status } = await import('@/modules/pinia')
    const wrapper = mount(HomeView, {
      global: {
        plugins: [createTestingPinia()],
        stubs: {
          BsMessage: true,
          BsCard: { template: '<div><slot /></div>' }
        }
      }
    })

    await wrapper.vm.refresh()
    expect(status.load).toHaveBeenCalled()

    wrapper.unmount()
    expect(wrapper.exists()).toBe(false)
  })
})
