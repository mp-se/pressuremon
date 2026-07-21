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
    <p class="h3">Push - Bluetooth</p>
    <hr />

    <template v-if="global.feature.ble">
      <form @submit.prevent="save" class="needs-validation" novalidate>
        <div class="row">
          <div class="col-md-12">
            <BsInputRadio
              v-model="config.ble_format"
              :options="bleFormatOptions"
              label="Bluetooth data format"
              help="Select the type of bluetooth transmission used."
              :disabled="global.disabled"
            />
          </div>
          <div class="col-md-12">
            <p></p>
            <p>Changing bluetooth settings might require a restart to function properly</p>
          </div>
        </div>
        <div class="row gy-2">
          <div class="col-md-12">
            <hr />
          </div>
          <div class="col-md-3">
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
              &nbsp;Save
            </button>
          </div>
        </div>
      </form>
    </template>
    <template v-else>
      <div class="row">
        <div class="col-md-12">
          <p>Bluetooth is not available on this platform</p>
        </div>
      </div>
    </template>
  </div>
</template>

<script setup>
import { ref } from 'vue'
import { validateCurrentForm } from '@mp-se/espframework-ui-components'
import { global, config } from '@/modules/pinia'

const bleFormatOptions = ref([
  { label: 'Disabled', value: 0 },
  { label: 'Pressuremon iBeacon', value: 1 }
  // { label: 'Pressuremon Eddystone', value: 2 }
])

const save = async () => {
  if (!validateCurrentForm()) return

  global.clearMessages()
  await config.saveAll()
}
</script>
