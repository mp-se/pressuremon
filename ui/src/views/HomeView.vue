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
        {{ t('home.no_push_targets') }}
      </BsMessage>

      <BsMessage
        v-if="!status.self_check.sensor_configured"
        dismissable="true"
        message=""
        alert="danger"
      >
        {{ t('home.no_sensor_configured') }}
      </BsMessage>

      <BsMessage
        v-if="!status.self_check.sensor_connected"
        dismissable="true"
        message=""
        alert="danger"
      >
        {{ t('home.no_sensor_connected') }}
      </BsMessage>
    </template>

    <div v-if="status" class="container overflow-hidden text-center">
      <div class="row gy-4">
        <div class="col-md-4" v-if="status.pressure">
          <BsCard :header="t('home.measurement')" color="info" :title="t('home.pressure_title')">
            <p class="text-center">{{ status.pressure }} {{ status.pressure_unit }}</p>
          </BsCard>
        </div>

        <div class="col-md-4" v-if="status.pressure1">
          <BsCard :header="t('home.measurement')" color="info" :title="t('home.pressure2_title')">
            <p class="text-center">{{ status.pressure1 }} {{ status.pressure_unit }}</p>
          </BsCard>
        </div>

        <div class="col-md-4" v-if="status.temp && status.temp >= -270">
          <BsCard :header="t('home.measurement')" color="info" :title="t('home.temperature')">
            <p class="text-center">{{ status.temp }} {{ status.temp_unit }}</p>
          </BsCard>
        </div>

        <div class="col-md-4" v-if="status.self_check.battery_level">
          <BsCard :header="t('home.measurement')" color="info" :title="t('home.battery')">
            <p class="text-center">{{ status.battery }} V</p>
          </BsCard>
        </div>
        <div class="col-md-4" v-if="!status.self_check.battery_level">
          <BsCard :header="t('home.measurement')" :title="t('home.error')" :iserr="true" icon="bi-x-circle">
            <p class="text-center">{{ t('home.battery_invalid') }}</p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard :header="t('home.device')" :title="t('home.wifi')">
            <p class="text-center">{{ status.rssi }} dBm - {{ status.wifi_ssid }}</p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard :header="t('home.device')" :title="t('home.ip_address')">
            <p class="text-center">
              {{ status.ip }}
            </p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard :header="t('home.device')" :title="t('home.memory')">
            <p class="text-center">
              {{ t('home.memory_text', { free: status.free_heap, total: status.total_heap }) }}
            </p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard :header="t('home.device')" :title="t('home.software_version')">
            <p class="text-center">
              {{
                t('home.software_version_text', {
                  appVer: global.app_ver,
                  appBuild: global.app_build,
                  uiVersion: global.uiVersion,
                  uiBuild: global.uiBuild
                })
              }}
            </p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard :header="t('home.device')" :title="t('home.platform')">
            <p class="text-center">
              {{ t('home.platform_text') }}<span class="badge bg-secondary">{{ global.platform }}</span>{{
                t('home.board_text')
              }}<span class="badge bg-secondary">{{ global.board }}</span>
            </p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard :header="t('home.device')" :title="t('home.id')">
            <p class="text-center">{{ status.id }}</p>
          </BsCard>
        </div>

        <div class="col-md-4">
          <BsCard :header="t('home.device')" :title="t('home.force_config_mode')">
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
import { useI18n } from 'vue-i18n'
import { status, global, config } from '@/modules/pinia'
import { useTimers } from '@mp-se/espframework-ui-components'

const { t } = useI18n()
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
