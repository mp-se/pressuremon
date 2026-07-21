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
    <p class="h3">Push - Settings</p>
    <hr />

    <BsMessage v-if="config.sleep_interval < 300" dismissable="true" message="" alert="warning">
      A sleep-interval of less than 300 will reduce battery life, consider using 900
    </BsMessage>

    <form @submit.prevent="save" class="needs-validation" novalidate>
      <div class="row">
        <div class="col-md-6">
          <BsInputText
            v-model="config.token"
            maxlength="50"
            label="Token 1"
            help="Token can be used in the format template as a variable, some services use this for authentication"
            :disabled="global.disabled"
          />
        </div>
        <div class="col-md-6">
          <BsInputText
            v-model="config.token2"
            maxlength="50"
            label="Token 2"
            help="Token can be used in the format template as a variable, some services use this for authentication"
            :disabled="global.disabled"
          />
        </div>
        <div class="col-md-6">
          <BsInputNumber
            v-model="config.sleep_interval"
            :label="'Sleep interval' + sleepLabel"
            unit="s"
            min="10"
            max="3600"
            step="1"
            width="5"
            help="The number of seconds that the device will sleep between sensor readings. Recommended value is 300s"
            :disabled="global.disabled"
          />
        </div>

        <div class="col-md-6">
          <BsInputReadonly
            v-model="batteryLife"
            label="Estimated battery life"
            help="Estimated based on current platform, 2200mAh battery and 2 seconds runtime"
            :disabled="global.disabled"
          ></BsInputReadonly>
        </div>

        <div class="col-md-6">
          <BsInputNumber
            v-model="config.push_timeout"
            label="Push timeout"
            unit="s"
            min="10"
            max="60"
            step="1"
            width="5"
            help="The number of seconds that the device will wait until a remote service accepts the connection"
            :disabled="global.disabled"
          />
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
  </div>
</template>

<script setup>
import { onMounted, ref, watch } from 'vue'
import { validateCurrentForm } from '@mp-se/espframework-ui-components'
import { global, config } from '@/modules/pinia'
import { storeToRefs } from 'pinia'
import { logDebug, logError } from '@mp-se/espframework-ui-components'

const { sleep_interval } = storeToRefs(config)
const batteryLife = ref('')
const sleepLabel = ref('')

const save = async () => {
  if (!validateCurrentForm()) return

  await config.saveAll()
}

watch(sleep_interval, () => {
  createSleepLabel()
  calculateBatteryLife()
})

onMounted(() => {
  createSleepLabel()
  calculateBatteryLife()
})

const createSleepLabel = () => {
  const s = Math.floor(sleep_interval.value / 60) + ' min ' + (sleep_interval.value % 60) + ' sec'
  sleepLabel.value = '(' + s + ')'
}

const calculateBatteryLife = () => {
  // ESP8266 consumes between 140-170mA when WIFI is on. Deep sleep is 20uA.
  // MPU-6050 consumes 4mA
  // DS18B20 consumes 1mA
  // For this estimation we use an average of 160mA
  let pwrActive = 160 // mA per hour (120-170 mA)
  const pwrSleep = 15 // mA per day (include all pheripials as well)
  const batt = 2200 // mA
  let rt = 2 // Assume 2 seconds
  const ble = config.ble_format === 0 ? false : true
  const wifi =
    config.http_post_target.length +
      config.http_post2_target.length +
      config.http_get_target.length +
      config.influxdb2_target.length +
      config.mqtt_target.length >
    0
      ? true
      : false

  if (!wifi && !ble) {
    logError(
      'PushSettingsView.calculateBatteryLife()',
      'No push targets defined, cannot estimate battery life'
    )
    return
  }

  if (wifi) {
    switch (global.platform) {
      case 'ESP32C3':
        pwrActive = 320 // mA per hour (290-350 mA)
        break
      case 'ESP32S2':
        pwrActive = 280 // mA per hour (260-300 mA)
        break
      case 'ESP32S3':
        pwrActive = 300 // mA per hour (285-355 mA)
        break
      default:
        logError('PushSettingsView.calculateBatteryLife()', 'Unknown platform', global.platform)
        break
    }
  } else {
    switch (global.platform) {
      case 'ESP32C3':
      case 'ESP32S2':
      case 'ESP32S3':
        pwrActive = 180
        break
      default:
        logError('PushSettingsView.calculateBatteryLife()', 'Unknown platform', global.platform)
        break
    }
  }

  if (rt < 4) rt = 4 // Assume that this takes at least 4 seconds to read sensors and do push.

  // The deep sleep will consume approx 1mA per day.
  const powerPerDay =
    ((24 * 3600) / (config.sleep_interval + rt)) * (rt / 3600) * pwrActive + pwrSleep
  const days = batt / powerPerDay

  logDebug(
    'PushSettingsView.calculateBatteryLife()',
    'Estimated power per hour = ' + pwrActive.toString() + 'mA on platform = ' + global.platform
  )
  logDebug('PushSettingsView.calculateBatteryLife()', 'Estimated number of days = ' + days)

  batteryLife.value = Math.floor(days / 7) + ' weeks ' + Math.floor(days % 7) + ' days'
}
</script>
