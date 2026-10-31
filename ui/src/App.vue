<!--
  Pressuremon - Dual License

  Open Source Use  ->  GNU General Public License v3.0 (GPL v3)
  Commercial Use   ->  Commercial License Agreement

  Copyright (c) 2024-2026 Magnus

  For full license terms and conditions, see LICENSE and LICENSE_COMMERCIAL files.
  Source: https://github.com/mp-se/pressuremon and https://github.com/mp-se/pressuremon-ui
-->
<template>
  <dialog id="spinner" class="loading">
    <div class="container text-center">
      <div class="row align-items-center" style="height: 170px">
        <div class="col">
          <div class="spinner-border" role="status" style="width: 5rem; height: 5rem">
            <span class="visually-hidden">{{ t('app.loading') }}</span>
          </div>
        </div>
      </div>
    </div>
  </dialog>

  <div v-if="!global.initialized" class="container text-center">
    <BsMessage :message="t('app.initializing')" :dismissable="false" alert="info"></BsMessage>
  </div>

  <BsMenuBar
    v-if="global.initialized"
    :disabled="global.disabled"
    :config-changed="global.configChanged"
    brand="PressureMon"
    :menu-items="menuItems"
    :mdns="config.mdns"
    :dark-mode="config.dark_mode"
    @update:dark-mode="handleDarkModeUpdate"
  />

  <div class="container">
    <div>
      <p></p>
    </div>
    <BsMessage
      v-if="!status.connected"
      :message="t('app.no_response')"
      :dismissable="false"
      alert="danger"
    ></BsMessage>

    <BsMessage
      v-if="global.isError"
      :close="close"
      :dismissable="true"
      :message="global.messageError"
      alert="danger"
    />
    <BsMessage
      v-if="global.isWarning"
      :close="close"
      :dismissable="true"
      :message="global.messageWarning"
      alert="warning"
    />
    <BsMessage
      v-if="global.isSuccess"
      :close="close"
      :dismissable="true"
      :message="global.messageSuccess"
      alert="success"
    />
    <BsMessage
      v-if="global.isInfo"
      :close="close"
      :dismissable="true"
      :message="global.messageInfo"
      alert="info"
    />

    <BsMessage v-if="status.wifi_setup" :dismissable="false" alert="info">
      {{ t('app.wifi_setup_info') }}
      <router-link class="alert-link" to="/device/wifi">{{
        t('app.wifi_setup_link')
      }}</router-link>
      {{ t('app.wifi_setup_suffix') }}
    </BsMessage>
  </div>

  <router-view v-if="global.initialized" />
  <BsFooter v-if="global.initialized" :text="t('app.footer')" />
</template>

<script setup>
import { onMounted, watch, onBeforeMount, onBeforeUnmount, ref } from 'vue'
import { useI18n } from 'vue-i18n'
import { sharedHttpClient as http } from '@mp-se/espframework-ui-components'
import { global, status, config, saveConfigState } from './modules/pinia'
import { useTimers, logInfo, logError, version } from '@mp-se/espframework-ui-components'
import { items as menuItems } from './modules/router'
import { loadLocalePack } from '@/modules/localePacks'

const { t, locale } = useI18n()
const polling = ref(null)
const { createInterval, clearManagedInterval } = useTimers()

// Keep the active vue-i18n locale in sync with the device-stored config value.
// Non-English locales need their pack loaded from the device's LittleFS first
// (uploaded via Device Settings' language pack list); if that pack isn't
// installed, fall back to English rather than switching to a locale with no
// messages loaded.
watch(
  () => config.locale,
  async (newValue) => {
    if (!newValue) return
    if (newValue === 'en') {
      locale.value = 'en'
      return
    }
    const loaded = await loadLocalePack(newValue)
    if (loaded) {
      locale.value = newValue
    } else {
      locale.value = 'en'
      global.messageWarning = t('app.err_locale_pack_missing')
    }
  },
  { immediate: true }
)

const close = (alert) => {
  if (alert == 'danger') global.messageError = ''
  else if (alert == 'warning') global.messageWarning = ''
  else if (alert == 'success') global.messageSuccess = ''
  else if (alert == 'info') global.messageInfo = ''
}

// Handle dark mode changes
const handleDarkModeUpdate = (newValue) => {
  // update the store value
  config.dark_mode = newValue
  // fallback: ensure the attribute is set on the document root so Bootstrap theme rules apply
  try {
    const root = document.documentElement
    if (newValue) root.setAttribute('data-bs-theme', 'dark')
    else root.setAttribute('data-bs-theme', 'light')
  } catch (e) {
    console.error('Failed to set data-bs-theme on documentElement', e)
  }
}

// Watch for changes to config.dark_mode and apply the selected theme.
watch(() => config.dark_mode, handleDarkModeUpdate)

const handleDisabledCursor = () => {
  if (global.disabled) document.body.style.cursor = 'wait'
  else document.body.style.cursor = 'default'
}

watch(() => global.disabled, handleDisabledCursor)

function ping() {
  ;(async () => {
    const ok = await http.ping()
    status.connected = ok
  })()
}

onBeforeMount(() => {
  polling.value = createInterval(ping, 7000)
})

onBeforeUnmount(() => {
  clearManagedInterval(polling.value)
})

onMounted(async () => {
  logInfo('App.onMounted()', `Using espframework version ${version}`)

  if (!global.initialized) {
    await initializeApp()
  }
})

async function initializeApp() {
  try {
    showSpinner()

    // Step 1: Authenticate with device (http client owns token)
    const base = btoa('pressuremon:password')
    const authOk = await http.auth(base)
    if (!authOk) {
      global.messageError = t('app.err_auth_failed')
      return
    }

    // Step 2: Load feature flags
    const globalSuccess = await global.load()
    if (!globalSuccess) {
      global.messageError = t('app.err_load_features')
      return
    }

    // Step 3: Load device status
    const statusSuccess = await status.load()
    if (!statusSuccess) {
      global.messageError = t('app.err_load_status')
      return
    }

    // Step 4: Load configuration
    const configSuccess = await config.load()
    if (!configSuccess) {
      global.messageError = t('app.err_load_config')
      return
    }

    // Step 5: Load format templates
    const formatSuccess = await config.loadFormat()
    if (!formatSuccess) {
      global.messageError = t('app.err_load_format')
      return
    }

    // Success! Initialize the app
    saveConfigState()
    handleDarkModeUpdate(config.dark_mode)
    global.initialized = true
  } catch (error) {
    logError('App.initializeApp()', error)
    global.messageError = t('app.err_init_failed', { error: error.message })
  } finally {
    hideSpinner()
  }
}

function showSpinner() {
  document.querySelector('#spinner').showModal()
}

function hideSpinner() {
  document.querySelector('#spinner').close()
}
</script>

<style>
.loading {
  position: fixed;
  width: 200px;
  height: 200px;
  padding: 10px;
  top: 0;
  right: 0;
  bottom: 0;
  left: 0;
  border: 0;
}

dialog::backdrop {
  background-color: black;
  opacity: 60%;
}
</style>
