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
    <p class="h3">{{ t('device_sensor.title') }}</p>
    <hr />

    <form @submit.prevent="save" class="needs-validation" novalidate>
      <div class="row">
        <div class="col-md-12">
          <BsInputRadio
            v-model="config.tempsensor_resolution"
            :options="tempsensorResolutionOptions"
            :label="t('device_sensor.ds18b20_label')"
            :help="t('device_sensor.ds18b20_help')"
            :disabled="global.disabled"
          ></BsInputRadio>
        </div>

        <div class="col-md-6">
          <BsInputNumber
            v-model="config.temp_adjustment_value"
            :unit="'°' + config.temp_unit"
            :label="t('device_sensor.temp_adjustment_label')"
            min="-10"
            max="10"
            step=".01"
            width="6"
            :help="t('device_sensor.temp_adjustment_help')"
            :disabled="global.disabled"
          ></BsInputNumber>
        </div>

        <div class="col-md-12">
          <hr />
        </div>

        <div class="col-md-6">
          <BsSelect
            v-model="config.sensor_type"
            :label="t('device_sensor.sensor_label')"
            :options="pressureSensorOptions"
            :disabled="global.disabled"
          />
        </div>

        <div class="col-md-6">
          <BsSelect
            v-model="config.sensor1_type"
            :label="t('device_sensor.sensor2_label')"
            :options="pressureSensorOptions"
            :disabled="global.disabled"
            v-if="status.max_sensors > 1"
          />
        </div>

        <div class="col-md-6">
          <BsInputNumber
            v-model="config.pressure_adjustment"
            :label="t('device_sensor.pressure_adjustment_label')"
            min="-1000"
            max="1000"
            step=".0001"
            width="6"
            :unit="config.pressure_unit"
            :help="t('device_sensor.pressure_adjustment_help')"
            :disabled="global.disabled || config.sensor_type < 1"
          >
          </BsInputNumber>
        </div>

        <div class="col-md-6">
          <BsInputNumber
            v-model="config.pressure1_adjustment"
            :label="t('device_sensor.pressure_adjustment2_label')"
            min="-1000"
            max="1000"
            step=".0001"
            width="6"
            :unit="config.pressure_unit"
            :help="t('device_sensor.pressure_adjustment_help')"
            :disabled="global.disabled || config.sensor1_type < 1"
            v-if="status.max_sensors > 1"
          >
          </BsInputNumber>
        </div>

        <div class="col-md-6">
          <BsInputNumber
            v-model="config.custom_min_voltage"
            :label="t('device_sensor.analog_min_voltage_label')"
            min="0"
            max="5.000"
            step=".001"
            width="6"
            unit="V"
            :disabled="global.disabled"
            v-if="config.sensor_type == 1000"
          >
          </BsInputNumber>
        </div>
        <div class="col-md-6">
          <BsInputNumber
            v-model="config.custom1_min_voltage"
            :label="t('device_sensor.analog_min_voltage2_label')"
            min="0"
            max="5.000"
            step=".001"
            width="6"
            unit="V"
            :disabled="global.disabled"
            v-if="status.max_sensors > 1 && config.sensor1_type == 1000"
          >
          </BsInputNumber>
        </div>

        <div class="col-md-6">
          <BsInputNumber
            v-model="config.custom_max_voltage"
            :label="t('device_sensor.analog_max_voltage_label')"
            min="0"
            max="5.000"
            step=".001"
            width="6"
            unit="V"
            :disabled="global.disabled"
            v-if="config.sensor_type == 1000"
          >
          </BsInputNumber>
        </div>
        <div class="col-md-6">
          <BsInputNumber
            v-model="config.custom1_max_voltage"
            :label="t('device_sensor.analog_max_voltage2_label')"
            min="0"
            max="5.000"
            step=".001"
            width="6"
            unit="V"
            :disabled="global.disabled"
            v-if="status.max_sensors > 1 && config.sensor1_type == 1000"
          >
          </BsInputNumber>
        </div>

        <div class="col-md-6">
          <BsInputNumber
            v-model="config.custom_min_pressure"
            :label="t('device_sensor.analog_min_pressure_label')"
            min="0"
            max="10000"
            step=".001"
            width="6"
            unit="kPa"
            :disabled="global.disabled"
            v-if="config.sensor_type == 1000"
          >
          </BsInputNumber>
        </div>
        <div class="col-md-6">
          <BsInputNumber
            v-model="config.custom1_min_pressure"
            :label="t('device_sensor.analog_min_pressure2_label')"
            min="0"
            max="10000"
            step=".001"
            width="6"
            unit="kPa"
            :disabled="global.disabled"
            v-if="status.max_sensors > 1 && config.sensor1_type == 1000"
          >
          </BsInputNumber>
        </div>

        <div class="col-md-6">
          <BsInputNumber
            v-model="config.custom_max_pressure"
            :label="t('device_sensor.analog_max_pressure_label')"
            min="0"
            max="10000"
            step=".001"
            width="6"
            unit="kPa"
            :disabled="global.disabled"
            v-if="config.sensor_type == 1000"
          >
          </BsInputNumber>
        </div>
        <div class="col-md-6">
          <BsInputNumber
            v-model="config.custom1_max_pressure"
            :label="t('device_sensor.analog_max_pressure2_label')"
            min="0"
            max="10000"
            step=".001"
            width="6"
            unit="kPa"
            :disabled="global.disabled"
            v-if="status.max_sensors > 1 && config.sensor1_type == 1000"
          >
          </BsInputNumber>
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
            &nbsp;{{ t('device_sensor.save') }}</button
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
            &nbsp;{{ t('device_sensor.restart') }}</button
          >&nbsp;

          <button
            @click="calibrate"
            type="button"
            class="btn btn-secondary"
            :disabled="global.disabled || !status.self_check.sensor_connected"
          >
            <span
              class="spinner-border spinner-border-sm"
              role="status"
              aria-hidden="true"
              v-show="global.disabled"
            ></span>
            &nbsp;{{ t('device_sensor.calibrate') }}&nbsp;<span
              v-if="badge.deviceSensorCalibratedBadge()"
              class="badge text-bg-danger rounded-circle"
              >1</span
            ></button
          >&nbsp;
        </div>
      </div>
    </form>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { useI18n } from 'vue-i18n'
import { validateCurrentForm } from '@mp-se/espframework-ui-components'
import { global, config, status } from '@/modules/pinia'
import { resolveMessage } from '@/modules/utils'
import * as badge from '@/modules/badge'
import { logDebug, logError, logInfo } from '@mp-se/espframework-ui-components'
import { useTimers, sharedHttpClient as http } from '@mp-se/espframework-ui-components'

const { t } = useI18n()
const { createTimeout } = useTimers()

const pressureSensorOptions = ref([
  { label: t('device_sensor.unused_option'), value: 0 }, // None selected

  { label: 'XIDIBEI XDB401 IIC 0.0 - 0.2 MPa', value: 1 }, // 2 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 0.4 MPa', value: 2 }, // 4 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 0.5 MPa', value: 3 }, // 5 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 0.6 MPa', value: 4 }, // 6 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 0.8 MPa', value: 5 }, // 8 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 1 MPa', value: 6 }, // 10 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 1.2 MPa', value: 7 }, // 12 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 1.5 MPa', value: 8 }, // 15 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 1.6 MPa', value: 9 }, // 16 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 2 MPa', value: 10 }, // 20 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 2.5 MPa', value: 11 }, // 25 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 3 MPa', value: 12 }, // 30 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 3.5 MPa', value: 13 }, // 35 bar
  { label: 'XIDIBEI XDB401 IIC 0.0 - 4 MPa', value: 14 } // 40 bar

  /*
  { label: 'XIDIBEI XDB401 Analog 0.0 - 0.2 MPa', value: 101 }, // 2 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 0.4 MPa', value: 102 }, // 4 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 0.5 MPa', value: 103 }, // 5 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 0.6 MPa', value: 104 }, // 6 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 0.8 MPa', value: 105 }, // 8 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 1 MPa', value: 106 }, // 10 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 1.2 MPa', value: 107 }, // 12 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 1.5 MPa', value: 108 }, // 15 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 1.6 MPa', value: 109 }, // 16 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 2 MPa', value: 100 }, // 20 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 2.5 MPa', value: 101 }, // 25 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 3 MPa', value: 102 }, // 30 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 3.5 MPa', value: 103 }, // 35 bar
  { label: 'XIDIBEI XDB401 Analog 0.0 - 4 MPa', value: 104 } // 40 bar
*/
])

const tempsensorResolutionOptions = ref([
  { label: '0.5°C (93 ms)', value: 9 },
  { label: '0.25°C (187 ms)', value: 10 },
  { label: '0.125°C (375 ms)', value: 11 },
  { label: '0.0625°C (850 ms)', value: 12 }
])

onMounted(() => {
  logDebug('DeviceSensorView.onMounted()')

  if (status.adc_found) {
    pressureSensorOptions.value.push({ label: t('device_sensor.custom_analog_option'), value: 1000 })
  }
})

const calibrate = async () => {
  global.clearMessages()
  global.disabled = true
  logInfo('DeviceSensorView.calibrate()', 'Starting calibration')

  try {
    // Start calibration using shared http client
    const startRes = await http.request('api/calibrate', { method: 'GET' })

    if (!startRes || startRes.ok === false) {
      const body = startRes ? await startRes.text().catch(() => null) : null
      global.messageError = t('device_sensor.err_start_calibration')
      logError(
        'DeviceSensorView.calibrate()',
        'Start calibration failed',
        startRes
          ? `HTTP ${startRes.status}: ${startRes.statusText}` + (body ? ` - ${body}` : '')
          : startRes
      )
      return { success: false }
    }

    // Poll for calibration completion
    while (true) {
      // Wait a short delay between polls
      await new Promise((r) => createTimeout(r, 2000))

      try {
        const statusJson = await http.getJson('api/calibrate/status')

        logDebug('DeviceSensorView.calibrate()', statusJson)

        if (statusJson.status) {
          // still running
          continue
        }

        if (!statusJson.success) {
          global.messageError = resolveMessage(statusJson.message_code, statusJson.message)
          return { success: false }
        }

        // Completed - reload config
        const configSuccess = await config.load()
        if (configSuccess) {
          global.messageSuccess = resolveMessage(statusJson.message_code, statusJson.message)
          return { success: true }
        } else {
          global.messageError = t('device_sensor.err_load_config')
          return { success: false }
        }
      } catch (err) {
        logError('DeviceSensorView.calibrate()', err)
        global.messageError = t('device_sensor.err_get_status')
        return { success: false }
      }
    }
  } catch (err) {
    logError('DeviceSensorView.calibrate()', err)
    global.messageError = err.message || t('device_sensor.err_calibration_unexpected')
    return { success: false }
  } finally {
    global.disabled = false
  }
}

const save = async () => {
  if (!validateCurrentForm()) return

  global.clearMessages()
  await config.saveAll()
}
</script>
