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
    <p class="h3">{{ t('support.title') }}</p>
    <hr />
    <div class="row">
      <p>{{ t('support.intro') }}</p>
    </div>
    <div class="row">
      <div class="col-md-4">
        <a
          class="link-primary link-offset-2 link-underline-opacity-25 link-underline-opacity-100-hover"
          href="https://github.com/mp-se/pressuremon"
          target="_blank"
          >{{ t('support.link_issues') }}</a
        >
      </div>
      <div class="col-md-4">
        <a
          class="link-primary link-offset-2 link-underline-opacity-25 link-underline-opacity-100-hover"
          href="https://www.homebrewtalk.com/"
          target="_blank"
          >{{ t('support.link_discuss') }}</a
        >
      </div>
    </div>

    <hr />
    <div class="row">
      <div class="col">
        <p>
          {{ t('support.platform_label') }}
          <span class="badge bg-secondary">{{ global.platform }}</span>
          {{ t('support.firmware_label') }}
          <span class="badge bg-secondary">{{ global.app_ver }} ({{ global.app_build }})</span>
          {{ t('support.ui_label') }}
          <span class="badge bg-secondary">{{ global.uiVersion }} ({{ global.uiBuild }})</span>
        </p>
      </div>
    </div>
    <hr />

    <div class="row">
      <div class="col-md-12">
        <button @click="viewLogs" type="button" class="btn btn-primary" :disabled="global.disabled">
          <span
            class="spinner-border spinner-border-sm"
            role="status"
            aria-hidden="true"
            v-show="global.disabled"
          ></span>
          &nbsp;{{ t('support.view_logs') }}</button
        >&nbsp;

        <button
          @click="removeLogs"
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
          &nbsp;{{ t('support.erase_logs') }}</button
        >&nbsp;

        <button
          @click="hardwareScan"
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
          &nbsp;{{ t('support.hardware_scan') }}</button
        >&nbsp;

        <button
          @click="showHelp = !showHelp"
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
          &nbsp;{{ t('support.toggle_help') }}</button
        >&nbsp;
      </div>
    </div>

    <div class="row">
      <div class="col">
        <p></p>
      </div>
    </div>
    <div class="row">
      <div class="col">
        <pre>{{ logData }}</pre>
      </div>
      <div class="form-text">{{ t('support.log_hint') }}</div>
    </div>

    <div class="row" v-if="showHelp">
      <div class="col-md-12">
        <p></p>
      </div>
      <div class="col-md-12">
        {{ t('support.http_error_title') }}
        <li v-for="key in httpErrorKeys" :key="key">{{ t('support.' + key) }}</li>
        <br />
        {{ t('support.mqtt_conn_title') }}
        <li v-for="key in mqttConnErrorKeys" :key="key">{{ t('support.' + key) }}</li>
        <br />
        {{ t('support.mqtt_push_title') }}
        <li v-for="key in mqttPushErrorKeys" :key="key">{{ t('support.' + key) }}</li>
        <br />
        {{ t('support.wifi_error_title') }}
        <li v-for="key in wifiErrorKeys" :key="key">{{ t('support.' + key) }}</li>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue'
import { useI18n } from 'vue-i18n'
import { config, global } from '@/modules/pinia'
import { logDebug } from '@mp-se/espframework-ui-components'

const { t } = useI18n()
const logData = ref('')
const showHelp = ref(false)

const httpErrorKeys = ['http_400', 'http_401', 'http_403', 'http_404']
const mqttConnErrorKeys = [
  'mqtt_conn_1',
  'mqtt_conn_2',
  'mqtt_conn_3',
  'mqtt_conn_4',
  'mqtt_conn_5',
  'mqtt_conn_6',
  'mqtt_conn_7',
  'mqtt_conn_8',
  'mqtt_conn_9',
  'mqtt_conn_10',
  'mqtt_conn_11',
  'mqtt_conn_100'
]
const mqttPushErrorKeys = [
  'mqtt_push_1',
  'mqtt_push_2',
  'mqtt_push_3',
  'mqtt_push_4',
  'mqtt_push_5',
  'mqtt_push_6',
  'mqtt_push_10',
  'mqtt_push_11'
]
const wifiErrorKeys = ['wifi_1', 'wifi_4', 'wifi_5', 'wifi_6', 'wifi_7']

function fetchLog(file, callback) {
  const data = {
    command: 'get',
    file: file
  }

  config.sendFilesystemRequest(data, (success, text) => {
    if (success) {
      logDebug('SupportView.fetchLog()', 'Fetching ' + file + ' completed')
      const list = text.split('\n')
      list.forEach((item) => {
        if (item.length) logData.value = item + '\n' + logData.value
      })
      callback(true)
    } else {
      callback(false)
    }
  })
}

function removeLog(file, callback) {
  const data = {
    command: 'del',
    file: file
  }

  config.sendFilesystemRequest(data, (success) => {
    callback(success)
  })
}

function viewLogs() {
  global.clearMessages()
  global.disabled = true
  logData.value = ''

  fetchLog('/error2.log', () => {
    fetchLog('/error.log', () => {
      global.disabled = false
    })
  })
}

function removeLogs() {
  global.clearMessages()
  global.disabled = true
  logData.value = ''

  removeLog('/error2.log', () => {
    removeLog('/error.log', () => {
      global.messageSuccess = t('support.logs_deleted')
      global.disabled = false
    })
  })
}

async function hardwareScan() {
  global.clearMessages()
  global.disabled = true
  logData.value = ''

  try {
    const data = await config.runHardwareScan()
    logData.value = data
  } catch {
    // Error already handled by runHardwareScan method
  } finally {
    global.disabled = false
  }
}
</script>
