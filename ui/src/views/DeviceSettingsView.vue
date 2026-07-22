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

        <div class="col-md-6">
          <BsInputRadio
            v-model="config.temp_unit"
            :options="tempOptions"
            :label="t('device_settings.temp_format_label')"
            width=""
            :disabled="global.disabled"
          ></BsInputRadio>
        </div>

        <div class="col-md-6">
          <BsInputRadio
            v-model="config.pressure_unit"
            :options="pressureOptions"
            :label="t('device_settings.pressure_format_label')"
            width=""
            :disabled="global.disabled"
          ></BsInputRadio>
        </div>

        <div class="col-md-12">
          <hr />
        </div>

        <div class="col-md-6">
          <BsInputRadio
            v-model="config.dark_mode"
            :options="uiOptions"
            :label="t('device_settings.ui_label')"
            width=""
            :disabled="global.disabled"
          ></BsInputRadio>
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
import { ref } from 'vue'
import { useI18n } from 'vue-i18n'
import { validateCurrentForm } from '@mp-se/espframework-ui-components'
import { global, config } from '@/modules/pinia'
import * as badge from '@/modules/badge'
import { resolveMessage } from '@/modules/utils'
import { logError, logInfo } from '@mp-se/espframework-ui-components'
import { useFetch, useTimers } from '@mp-se/espframework-ui-components'

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
