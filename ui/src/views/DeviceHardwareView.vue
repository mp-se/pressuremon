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
    <p class="h3">{{ t('device_hardware.title') }}</p>
    <hr />

    <form @submit.prevent="save" class="needs-validation" novalidate>
      <div class="row">
        <div class="col-md-6">
          <BsInputNumber
            v-model="config.voltage_factor"
            :label="t('device_hardware.voltage_factor_label')"
            min="0"
            max="6"
            step=".01"
            width="4"
            :unit="voltage"
            :help="t('device_hardware.voltage_factor_help')"
            :disabled="global.disabled"
          >
          </BsInputNumber>
        </div>
        <div class="col-md-6">
          <BsInputNumber
            v-model="config.voltage_config"
            unit="V"
            :label="t('device_hardware.voltage_config_label')"
            min="3"
            max="6"
            step=".01"
            width="4"
            :help="t('device_hardware.voltage_config_help')"
            :disabled="global.disabled"
          ></BsInputNumber>
        </div>

        <div class="col-md-6">
          <BsInputSwitch
            v-model="config.battery_saving"
            :label="t('device_hardware.battery_saving_label')"
            :help="t('device_hardware.battery_saving_help')"
            :disabled="global.disabled"
          ></BsInputSwitch>
        </div>

        <div class="col-md-6">
          <BsSelect
            v-model="config.battery_type"
            :label="t('device_hardware.battery_type_label')"
            :help="t('device_hardware.battery_type_help')"
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
            &nbsp;{{ t('device_hardware.save') }}</button
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
            &nbsp;{{ t('device_hardware.restart') }}</button
          >&nbsp;
        </div>
      </div>
    </form>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'
import { useI18n } from 'vue-i18n'
import { validateCurrentForm } from '@mp-se/espframework-ui-components'
import { global, config, status } from '@/modules/pinia'

const { t } = useI18n()

const batteryTypeOptions = ref([
  { label: t('device_hardware.battery_type_lipo'), value: 0 },
  { label: t('device_hardware.battery_type_liion'), value: 1 }
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
