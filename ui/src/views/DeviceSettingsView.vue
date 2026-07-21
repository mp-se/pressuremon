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
    <p class="h2">Device - Settings</p>
    <hr />

    <BsMessage v-if="config.mdns === ''" dismissable="true" message="" alert="warning">
      You need to define a mdns name for the device
    </BsMessage>

    <form @submit.prevent="saveSettings" class="needs-validation" novalidate>
      <div class="row">
        <div class="col-md-12">
          <BsInputText
            v-model="config.mdns"
            maxlength="63"
            minlength="1"
            label="MDNS"
            help="Enter device name used on the network, the suffix .local will be added to this name"
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
            label="Temperature Units"
            width=""
            :disabled="global.disabled"
          ></BsInputRadio>
        </div>

        <div class="col-md-6">
          <BsInputRadio
            v-model="config.pressure_unit"
            :options="pressureOptions"
            label="Pressure Units"
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
            label="User Interface"
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
            &nbsp;Save</button
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
            &nbsp;Restart device</button
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
            &nbsp;Restore factory defaults
          </button>
        </div>
      </div>
    </form>
  </div>
</template>

<script setup>
import { ref } from 'vue'
import { validateCurrentForm } from '@mp-se/espframework-ui-components'
import { global, config } from '@/modules/pinia'
import * as badge from '@/modules/badge'
import { logError, logInfo } from '@mp-se/espframework-ui-components'
import { useFetch, useTimers } from '@mp-se/espframework-ui-components'

const { managedFetch } = useFetch()
const { createTimeout } = useTimers()

const tempOptions = ref([
  { label: 'Celsius °C', value: 'C' },
  { label: 'Fahrenheit °F', value: 'F' }
])

const pressureOptions = ref([
  { label: 'PSI', value: 'PSI' },
  { label: 'kPA', value: 'kPa' },
  { label: 'Bar', value: 'Bar' }
])

const uiOptions = ref([
  { label: 'Day mode', value: false },
  { label: 'Dark mode', value: true }
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
      global.messageSuccess = json.message
      createTimeout(() => {
        location.reload(true)
      }, 2000)
    } else {
      global.messageError = json.message
      global.disabled = false
    }
  } catch (err) {
    logError('DeviceSettingsView.factory()', err)
    global.messageError = 'Failed to do factory restore'
    global.disabled = false
  }
}

const saveSettings = async () => {
  if (!validateCurrentForm()) return

  await config.saveAll()
}
</script>
