<!--
  Pressuremon - Dual License

  Open Source Use  ->  GNU General Public License v3.0 (GPL v3)
  Commercial Use   ->  Commercial License Agreement

  Copyright (c) 2024-2026 Magnus

  For full license terms and conditions, see LICENSE and LICENSE_COMMERCIAL files.
  Source: https://github.com/mp-se/pressuremon and https://github.com/mp-se/pressuremon-ui
-->
<template>
  <div class="container">
    <p></p>
    <p class="h2">{{ t('device_settings.title') }}</p>
    <hr />

    <BsMessage v-if="config.mdns === ''" dismissable="true" message="" alert="warning">
      {{ t('device_settings.mdns_warning') }}
    </BsMessage>

    <form @submit.prevent="saveSettings" class="needs-validation" novalidate>
      <div class="row">
        <div class="col-md-12">
          <BsInputText
            v-model="config.mdns"
            maxlength="63"
            minlength="1"
            :label="t('device_settings.mdns_label')"
            :help="t('device_settings.mdns_help')"
            :badge="badge.deviceMdnsBadge()"
            :disabled="global.disabled"
          >
          </BsInputText>
        </div>

        <div class="col-md-12">
          <hr />
        </div>

        <div class="col-md-4">
          <BsInputRadio
            v-model="config.temp_unit"
            :options="tempOptions"
            :label="t('device_settings.temp_format_label')"
            width=""
            :disabled="global.disabled"
          ></BsInputRadio>
        </div>

        <div class="col-md-4">
          <BsInputRadio
            v-model="config.pressure_unit"
            :options="pressureOptions"
            :label="t('device_settings.pressure_format_label')"
            width=""
            :disabled="global.disabled"
          ></BsInputRadio>
        </div>

        <div class="col-md-4">
          <BsInputRadio
            v-model="config.dark_mode"
            :options="uiOptions"
            :label="t('device_settings.ui_label')"
            width=""
            :disabled="global.disabled"
          ></BsInputRadio>
        </div>

        <div class="col-md-12">
          <hr />
        </div>

        <div class="col-md-4">
          <BsSelect
            v-model="config.locale"
            :options="localeOptions"
            :label="t('device_settings.locale_label')"
            :disabled="global.disabled"
          ></BsSelect>
        </div>

        <div class="col-md-12" v-if="global.ui.enableLanguageDownload && availableLanguages.length > 0">
          <hr />
        </div>

        <div class="col-md-12" v-if="global.ui.enableLanguageDownload && availableLanguages.length > 0">
          <h6>{{ t('language_packs.available_title') }}</h6>
          <div class="button-group">
            <template v-for="entry in availableLanguages" :key="entry.code">
              <button
                type="button"
                class="btn btn-sm"
                :class="isInstalled(entry.code) ? 'btn-primary' : 'btn-outline-secondary'"
                :disabled="global.disabled || installingCode !== null"
                @click.prevent="toggleLanguage(entry)"
              >
                <span
                  class="spinner-border spinner-border-sm"
                  role="status"
                  aria-hidden="true"
                  v-show="installingCode === entry.code"
                ></span>
                {{ entry.name }}</button
              >&nbsp;
            </template>
          </div>
          <div v-if="installingCode !== null && installProgress > 0" class="col-md-6">
            <p></p>
            <BsProgress :progress="installProgress"></BsProgress>
          </div>
        </div>
      </div>

      <div class="row gy-2">
        <div class="col-md-12">
          <hr />
        </div>
        <div class="col-md-12">
          <button
            type="submit"
            class="btn btn-primary w-2"
            :disabled="global.disabled || !global.configChanged"
          >
            <span
              class="spinner-border spinner-border-sm"
              role="status"
              aria-hidden="true"
              v-show="global.disabled"
            ></span>
            &nbsp;{{ t('device_settings.save') }}</button
          >&nbsp;

          <button
            @click="config.restart()"
            type="button"
            class="btn btn-secondary"
            :disabled="global.disabled"
          >
            <span
              class="spinner-border spinner-border-sm"
              role="status"
              aria-hidden="true"
              v-show="global.disabled"
            ></span>
            &nbsp;{{ t('device_settings.restart') }}</button
          >&nbsp;

          <button
            @click="factory"
            type="button"
            class="btn btn-secondary"
            :disabled="global.disabled"
          >
            <span
              class="spinner-border spinner-border-sm"
              role="status"
              aria-hidden="true"
              v-show="global.disabled"
            ></span>
            &nbsp;{{ t('device_settings.factory_defaults') }}
          </button>
        </div>
      </div>
    </form>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue'
import { useI18n } from 'vue-i18n'
import { BsProgress, validateCurrentForm } from '@mp-se/espframework-ui-components'
import { global, config } from '@/modules/pinia'
import * as badge from '@/modules/badge'
import { resolveMessage } from '@/modules/utils'
import { logError, logInfo } from '@mp-se/espframework-ui-components'
import { useFetch, useTimers } from '@mp-se/espframework-ui-components'
import {
  installPackFromUrl,
  listInstalledPacks,
  removeLocalePack,
  loadLocalePackWithRetry
} from '@/modules/localePacks'
import { fetchManifest } from '@/lib/langpacks'

const { t } = useI18n()
const { managedFetch } = useFetch()
const { createTimeout } = useTimers()

const tempOptions = ref([
  { label: t('device_settings.temp_celsius'), value: 'C' },
  { label: t('device_settings.temp_fahrenheit'), value: 'F' }
])

const pressureOptions = ref([
  { label: t('device_settings.pressure_psi'), value: 'PSI' },
  { label: t('device_settings.pressure_kpa'), value: 'kPa' },
  { label: t('device_settings.pressure_bar'), value: 'Bar' }
])

const uiOptions = ref([
  { label: t('device_settings.ui_day_mode'), value: false },
  { label: t('device_settings.ui_dark_mode'), value: true }
])

// Convenience list of downloadable language packs, read from the same
// version.json-style manifest already used for firmware update checks
// (hosted alongside gravitymon's, keyed by product name).
const availableLanguages = ref([])
const installedCodes = ref([])
const installingCode = ref(null)
const installProgress = ref(0)

const isInstalled = (code) => installedCodes.value.includes(code)

// The dropdown only lists languages that are actually installed on the
// device (English is always available since it ships embedded) - selecting
// a language with no pack installed would just fall back to English anyway.
const localeOptions = computed(() => {
  const options = [{ label: 'English', value: 'en' }]
  for (const code of installedCodes.value) {
    const entry = availableLanguages.value.find((lang) => lang.code === code)
    options.push({ label: entry ? entry.name : code, value: code })
  }
  return options
})

const refreshInstalledCodes = async () => {
  installedCodes.value = await listInstalledPacks()
}

onMounted(async () => {
  await refreshInstalledCodes()

  if (!global.ui.enableLanguageDownload) return

  try {
    const manifest = await fetchManifest('pressuremon', global.app_ver)
    availableLanguages.value = (manifest.packs || []).map((p) => ({
      code: p.lang,
      name: p.name || p.lang,
      file: p.filename,
      url: p.url // resolved to absolute by fetchManifest
    }))
  } catch (error) {
    logError('DeviceSettingsView.loadAvailableLanguages()', error)
    availableLanguages.value = []
  }
})

const toggleLanguage = async (entry) => {
  if (installingCode.value !== null) return

  installingCode.value = entry.code
  installProgress.value = 0
  global.clearMessages()

  try {
    if (isInstalled(entry.code)) {
      const res = await removeLocalePack(entry.code)
      if (res && res.success) {
        global.messageSuccess = t('language_packs.delete_success')
        await refreshInstalledCodes()
      } else {
        global.messageError = t('language_packs.err_delete_failed')
      }
    } else {
      await installPackFromUrl(entry.url, entry, {
        onProgress: (percent) => {
          installProgress.value = Math.round(percent)
        }
      })
      await refreshInstalledCodes()

      const loaded = await loadLocalePackWithRetry(entry.code)
      if (loaded) {
        global.messageSuccess = t('language_packs.install_success')
      } else {
        // Uploaded but failed to load back - treat as a failed install rather
        // than silently leaving a broken/incomplete file on the device.
        await removeLocalePack(entry.code)
        await refreshInstalledCodes()
        global.messageError = t('language_packs.err_install_failed')
      }
    }
  } catch (error) {
    logError('DeviceSettingsView.toggleLanguage()', error)
    global.messageError = t('language_packs.err_install_failed')
  } finally {
    installingCode.value = null
    installProgress.value = 0
  }
}

const factory = async () => {
  global.clearMessages()
  logInfo('DeviceSettingsView.factory()', 'Sending /api/factory')
  global.disabled = true

  try {
    const response = await managedFetch(global.baseURL + 'api/factory', {
      headers: { Authorization: global.token },
      signal: AbortSignal.timeout(global.fetchTimeout)
    })

    const json = await response.json()

    if (json.success == true) {
      global.messageSuccess = resolveMessage(json.message_code, json.message)
      createTimeout(() => {
        location.reload(true)
      }, 2000)
    } else {
      global.messageError = resolveMessage(json.message_code, json.message)
      global.disabled = false
    }
  } catch (err) {
    logError('DeviceSettingsView.factory()', err)
    global.messageError = t('device_settings.err_factory_restore')
    global.disabled = false
  }
}

const saveSettings = async () => {
  if (!validateCurrentForm()) return

  await config.saveAll()
}
</script>
