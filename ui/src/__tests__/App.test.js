import { mount, shallowMount } from '@vue/test-utils'
import App from '../App.vue'
import { createTestingPinia } from '../tests/testUtils'

describe('App.vue (smoke)', () => {
  it('mounts without error', () => {
    const pinia = createTestingPinia()
    const wrapper = shallowMount(App, { global: { plugins: [pinia] } })
    expect(wrapper.exists()).toBe(true)
  })
})

describe('App.vue (function tests)', () => {
  beforeEach(() => vi.clearAllMocks())

  it('close clears messageError when alert is danger', async () => {
    const { global } = await import('@/modules/pinia')
    global.messageError = 'Test error'
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    wrapper.vm.close('danger')
    expect(global.messageError).toBe('')
  })

  it('close clears messageWarning when alert is warning', async () => {
    const { global } = await import('@/modules/pinia')
    global.messageWarning = 'Test warning'
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    wrapper.vm.close('warning')
    expect(global.messageWarning).toBe('')
  })

  it('close clears messageSuccess when alert is success', async () => {
    const { global } = await import('@/modules/pinia')
    global.messageSuccess = 'Test success'
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    wrapper.vm.close('success')
    expect(global.messageSuccess).toBe('')
  })

  it('close clears messageInfo when alert is info', async () => {
    const { global } = await import('@/modules/pinia')
    global.messageInfo = 'Test info'
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    wrapper.vm.close('info')
    expect(global.messageInfo).toBe('')
  })

  it('handleDarkModeUpdate sets config.dark_mode to true', async () => {
    const { config } = await import('@/modules/pinia')
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    wrapper.vm.handleDarkModeUpdate(true)
    expect(config.dark_mode).toBe(true)
  })

  it('handleDarkModeUpdate sets config.dark_mode to false', async () => {
    const { config } = await import('@/modules/pinia')
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    wrapper.vm.handleDarkModeUpdate(false)
    expect(config.dark_mode).toBe(false)
  })

  it('showSpinner shows the spinner dialog', () => {
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    const mockShowModal = vi.fn()
    vi.spyOn(document, 'querySelector').mockReturnValue({ showModal: mockShowModal })
    wrapper.vm.showSpinner()
    expect(mockShowModal).toHaveBeenCalled()
  })

  it('hideSpinner closes the spinner dialog', () => {
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    const mockClose = vi.fn()
    vi.spyOn(document, 'querySelector').mockReturnValue({ close: mockClose })
    wrapper.vm.hideSpinner()
    expect(mockClose).toHaveBeenCalled()
  })

  it('ping calls http.ping and updates status.connected', async () => {
    const { status } = await import('@/modules/pinia')
    const httpModule = await import('@mp-se/espframework-ui-components')
    const httpMock = httpModule.sharedHttpClient
    httpMock.ping = vi.fn(async () => true)
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    wrapper.vm.ping()
    await wrapper.vm.$nextTick()
    await new Promise((resolve) => setTimeout(resolve, 10))
    expect(httpMock.ping).toHaveBeenCalled()
    expect(status.connected).toBe(true)
  })

  it('handleDarkModeUpdate sets data-bs-theme attribute to dark', () => {
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    const root = document.documentElement
    wrapper.vm.handleDarkModeUpdate(true)
    expect(root.getAttribute('data-bs-theme')).toBe('dark')
  })

  it('handleDarkModeUpdate sets data-bs-theme attribute to light', () => {
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    const root = document.documentElement
    wrapper.vm.handleDarkModeUpdate(false)
    expect(root.getAttribute('data-bs-theme')).toBe('light')
  })

  it('handleDarkModeUpdate handles document attribute errors', () => {
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    const consoleErrorSpy = vi.spyOn(console, 'error').mockImplementation(() => {})
    vi.spyOn(document.documentElement, 'setAttribute').mockImplementationOnce(() => {
      throw new Error('DOM error')
    })
    expect(() => wrapper.vm.handleDarkModeUpdate(true)).not.toThrow()
    consoleErrorSpy.mockRestore()
    vi.restoreAllMocks()
  })

  it('handleDisabledCursor sets wait cursor when the app is busy', async () => {
    const { global } = await import('@/modules/pinia')
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    global.disabled = true

    wrapper.vm.handleDisabledCursor()

    expect(document.body.style.cursor).toBe('wait')
  })

  it('handleDisabledCursor resets the cursor when the app is idle', async () => {
    const { global } = await import('@/modules/pinia')
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    global.disabled = false

    wrapper.vm.handleDisabledCursor()

    expect(document.body.style.cursor).toBe('default')
  })

  it('unmounts cleanly after the polling lifecycle has been created', async () => {
    const { global } = await import('@/modules/pinia')
    global.initialized = true
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })

    expect(wrapper.exists()).toBe(true)
    wrapper.unmount()
    expect(wrapper.exists()).toBe(false)
  })

  it('renders the wifi setup message when the device is in setup mode', async () => {
    const { global, status } = await import('@/modules/pinia')
    global.initialized = true
    status.wifi_setup = true

    const wrapper = mount(App, {
      global: {
        plugins: [createTestingPinia()],
        stubs: {
          BsMessage: { template: '<div><slot />{{ message }}</div>', props: ['message'] },
          RouterLink: { template: '<a><slot /></a>' },
          BsMenuBar: true,
          BsFooter: true,
          RouterView: true
        }
      }
    })

    expect(wrapper.text()).toContain('Running in WIFI setup mode')
    status.wifi_setup = false
  })

  it('close leaves messages unchanged for unknown alert types', async () => {
    const { global } = await import('@/modules/pinia')
    global.messageInfo = 'keep me'
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })

    wrapper.vm.close('other')

    expect(global.messageInfo).toBe('keep me')
  })
})

describe('App.vue (initializeApp tests)', () => {
  let mockShowModal
  let mockClose
  let http
  let globalStore
  let statusStore
  let configStore

  beforeEach(async () => {
    vi.clearAllMocks()

    mockShowModal = vi.fn()
    mockClose = vi.fn()
    vi.spyOn(document, 'querySelector').mockReturnValue({
      showModal: mockShowModal,
      close: mockClose
    })

    const pinia = await import('@/modules/pinia')
    globalStore = pinia.global
    statusStore = pinia.status
    configStore = pinia.config

    const httpModule = await import('@mp-se/espframework-ui-components')
    http = httpModule.sharedHttpClient
    http.auth = vi.fn(async () => true)
    http.ping = vi.fn(async () => true)

    globalStore.initialized = false
    globalStore.load = vi.fn(async () => true)
    statusStore.load = vi.fn(async () => true)
    statusStore.connected = true
    configStore.load = vi.fn(async () => true)
    configStore.loadFormat = vi.fn(async () => true)
  })

  afterEach(() => {
    vi.restoreAllMocks()
  })

  it('initializeApp succeeds and sets global.initialized to true', async () => {
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    await wrapper.vm.initializeApp()
    expect(globalStore.initialized).toBe(true)
    expect(mockShowModal).toHaveBeenCalled()
    expect(mockClose).toHaveBeenCalled()
  })

  it('initializeApp sets messageError and returns when auth fails', async () => {
    http.auth = vi.fn(async () => false)
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    await wrapper.vm.initializeApp()
    expect(globalStore.messageError).toContain('authenticate')
    expect(globalStore.initialized).toBe(false)
  })

  it('initializeApp sets messageError and returns when global.load fails', async () => {
    globalStore.load = vi.fn(async () => false)
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    await wrapper.vm.initializeApp()
    expect(globalStore.messageError).toContain('feature flags')
    expect(globalStore.initialized).toBe(false)
  })

  it('initializeApp sets messageError and returns when status.load fails', async () => {
    statusStore.load = vi.fn(async () => false)
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    await wrapper.vm.initializeApp()
    expect(globalStore.messageError).toContain('status')
    expect(globalStore.initialized).toBe(false)
  })

  it('initializeApp sets messageError and returns when config.load fails', async () => {
    configStore.load = vi.fn(async () => false)
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    await wrapper.vm.initializeApp()
    expect(globalStore.messageError).toContain('configuration')
    expect(globalStore.initialized).toBe(false)
  })

  it('initializeApp sets messageError and returns when config.loadFormat fails', async () => {
    configStore.loadFormat = vi.fn(async () => false)
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    await wrapper.vm.initializeApp()
    expect(globalStore.messageError).toContain('format templates')
    expect(globalStore.initialized).toBe(false)
  })

  it('initializeApp sets messageError on unexpected exception', async () => {
    globalStore.load = vi.fn(async () => {
      throw new Error('unexpected')
    })
    const wrapper = shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    await wrapper.vm.initializeApp()
    expect(globalStore.messageError).toContain('Initialization failed')
  })

  it('onMounted calls initializeApp when not initialized', async () => {
    globalStore.initialized = false
    shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    await new Promise((resolve) => setTimeout(resolve, 10))
    expect(globalStore.initialized).toBe(true)
  })

  it('onMounted skips initializeApp when already initialized', async () => {
    globalStore.initialized = true
    shallowMount(App, { global: { plugins: [createTestingPinia()] } })
    await new Promise((resolve) => setTimeout(resolve, 10))
    expect(http.auth).not.toHaveBeenCalled()
  })
})
