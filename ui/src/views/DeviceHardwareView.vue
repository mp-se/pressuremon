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
    <p class="h3">Device - Hardware</p>
    <hr />

    <form @submit.prevent="save" class="needs-validation" novalidate>
      <div class="row">
        <div class="col-md-6">
          <BsInputNumber
            v-model="config.voltage_factor"
            label="Voltage factor"
            min="0"
            max="6"
            step=".01"
            width="4"
            :unit="voltage"
            help="Factor used to calculate the battery voltage. Can vary depending on the R2 value (0 to 6)"
            :disabled="global.disabled"
          >
          </BsInputNumber>
        </div>
        <div class="col-md-6">
          <BsInputNumber
            v-model="config.voltage_config"
            unit="V"
            label="Voltage config"
            min="3"
            max="6"
            step=".01"
            width="4"
            help="Over this level the device will always go into configuration mode, some batteries might have a higher voltage when fully charged (3 to 6)"
            :disabled="global.disabled"
          ></BsInputNumber>
        </div>

        <div class="col-md-6">
          <BsInputSwitch
            v-model="config.battery_saving"
            label="Battery saving"
            help="When active, the sleep interval will be changed to 1 hour when battery drops below 20% (3.73V)"
            :disabled="global.disabled"
          ></BsInputSwitch>
        </div>

        <div class="col-md-6">
          <BsSelect
            v-model="config.battery_type"
            label="Battery type"
            help="For correct calculation of remaning capacity and battery savings mode"
            :options="batteryTypeOptions"
            :disabled="global.disabled"
          />
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
        </div>
      </div>
    </form>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'
import { validateCurrentForm } from '@mp-se/espframework-ui-components'
import { global, config, status } from '@/modules/pinia'

const batteryTypeOptions = ref([
  { label: 'LiPo battery', value: 0 },
  { label: 'LithiumIon battery (18650)', value: 1 }
])

const voltage = computed(() => {
  return status.battery + ' V'
})

const save = async () => {
  if (!validateCurrentForm()) return

  global.clearMessages()
  await config.saveAll()
}
</script>
