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

    <template v-if="status">
      <BsMessage
        v-if="!status.self_check.push_targets"
        dismissable="true"
        message=""
        alert="danger"
      >
        No remote services are active. Check your push settings and enable at least one service.
      </BsMessage>

      <BsMessage
        v-if="!status.self_check.sensor_configured"
        dismissable="true"
        message=""
        alert="danger"
      >
        No sensor has been configured, update the hardware configuration.
      </BsMessage>

      <BsMessage
        v-if="!status.self_check.sensor_connected"
        dismissable="true"
        message=""
        alert="danger"
      >
        No sensor is detected. Try to reboot / power-off. If this persists, check for hardware
        issues.
      </BsMessage>
    </template>

    <div v-if="status" class="container overflow-hidden text-center">
      <div class="row gy-4">
        <div class="col-md-4" v-if="status.pressure">
          <BsCard header="Measurement" color="info" title="Pressure">
            <p class="text-center">{{ status.pressure }} {{ status.pressure_unit }}</p>
          </BsCard>
        </div>

        <div class="col-md-4" v-if="status.pressure1">
          <BsCard header="Measurement" color="info" title="Pressure 2">
            <p class="text-center">{{ status.pressure1 }} {{ status.pressure_unit }}</p>
          </BsCard>
        </div>

        <div class="col-md-4" v-if="status.temp && status.temp >= -270">
          <BsCard header="Measurement" color="info" title="Temperature">
            <p class="text-center">{{ status.temp }} {{ status.temp_unit }}</p>
          </BsCard>
        </div>

        <div class="col-md-4" v-if="status.self_check.battery_level">
          <BsCard header="Measurement" color="info" title="Battery">
            <p class="text-center">{{ status.battery }} V</p>
          </BsCard>
        </div>
        <div class="col-md-4" v-if="!status.self_check.battery_level">
          <BsCard header="Measurement" title="Error" :iserr="true" icon="bi-x-circle">
            <p class="text-center">Battery level not valid</p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard header="Device" title="WIFI">
            <p class="text-center">{{ status.rssi }} dBm - {{ status.wifi_ssid }}</p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard header="Device" title="IP Address">
            <p class="text-center">
              {{ status.ip }}
            </p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard header="Device" title="Memory">
            <p class="text-center">
              Free: {{ status.free_heap }} kb, Total: {{ status.total_heap }} kb
            </p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard header="Device" title="Software version">
            <p class="text-center">
              Firmware: {{ global.app_ver }} ({{ global.app_build }}) UI: {{ global.uiVersion }} ({{
                global.uiBuild
              }})
            </p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard header="Device" title="Platform">
            <p class="text-center">
              Platform: <span class="badge bg-secondary">{{ global.platform }}</span> Board:
              <span class="badge bg-secondary">{{ global.board }}</span>
            </p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard header="Device" title="ID">
            <p class="text-center">{{ status.id }}</p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard header="Device" title="Force config mode">
            <div class="d-flex justify-content-center">
              <div class="form-check form-switch" style="height: 0.7rem">
                <input
                  v-model="flag"
                  class="form-check-input p-2"
                  type="checkbox"
                  role="switch"
                  style="
                    transform: scale(1.5);
                    transform-origin: left center;
                    display: inline-block;
                  "
                />
              </div>
            </div>
          </BsCard>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, watch, onMounted, onBeforeMount, onBeforeUnmount } from 'vue'
import { status, global, config } from '@/modules/pinia'
import { useTimers } from '@mp-se/espframework-ui-components'

const polling = ref(null)
const flag = ref(false)
const { createInterval, clearManagedInterval } = useTimers()

watch(flag, async () => {
  await config.setSleepMode(flag.value)
})

async function refresh() {
  await status.load()
}

onMounted(() => {
  flag.value = status.sleep_mode
})

onBeforeMount(() => {
  refresh()
  polling.value = createInterval(refresh, 4000)
})

onBeforeUnmount(() => {
  clearManagedInterval(polling.value)
})
</script>

<style></style>
