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
    <p class="h3">Backup & Restore</p>
    <hr />

    <div class="row">
      <div class="col-md-12">
        <p>Create a backup of the device configuration and store this in a textfile</p>
      </div>

      <div class="col-md-12">
        <button
          @click="backup"
          type="button"
          class="btn btn-primary w-2"
          data-bs-toggle="tooltip"
          :disabled="global.disabled"
        >
          Create backup
        </button>
      </div>

      <div class="col-md-12">
        <hr />
      </div>

      <div class="col-md-12">
        <p>Restore a previous backup of the device configuration by uploading it.</p>
      </div>
    </div>

    <div class="row">
      <form @submit.prevent="restore">
        <div class="col-md-12">
          <BsFileUpload
            name="upload"
            id="upload"
            label="Select backup file"
            accept=".txt"
            :disabled="global.disabled"
            @change="onFileChange"
          >
          </BsFileUpload>
        </div>

        <div class="col-md-3">
          <p></p>
          <button
            type="submit"
            class="btn btn-primary"
            value="upload"
            data-bs-toggle="tooltip"
            :title="restoreButtonTooltip"
            :disabled="global.disabled || !fileSelected"
          >
            <span
              class="spinner-border spinner-border-sm"
              role="status"
              aria-hidden="true"
              v-show="global.disabled"
            ></span>
            &nbsp;Restore
          </button>
        </div>

        <div v-if="progress > 0" class="col-md-12">
          <p></p>
          <BsProgress :progress="progress"></BsProgress>
        </div>
      </form>
    </div>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'
import { global, config, getConfigChanges } from '@/modules/pinia'
import { logDebug, logError } from '@mp-se/espframework-ui-components'
import { BsFileUpload, BsProgress } from '@mp-se/espframework-ui-components'

const progress = ref(0)
const fileSelected = ref(false)

const restoreButtonTooltip = computed(() => {
  if (global.disabled) {
    return 'Restore in progress, please wait...'
  } else if (!fileSelected.value) {
    return 'Please select a backup file first'
  } else {
    return 'Upload the configuration to the device'
  }
})

function onFileChange(event) {
  const files = event.target.files
  fileSelected.value = files && files.length > 0
  logDebug('BackupView.onFileChange()', 'File selected:', fileSelected.value)
}

function backup() {
  let backup = {
    meta: {
      version: '0.5.0',
      software: 'PressureMon',
      created: new Date().toISOString().slice(0, 10)
    },
    config: JSON.parse(config.toJson())
  }

  logDebug('BackupView.backup()', backup)

  backup.config.http_post_format_pressure = encodeURIComponent(
    backup.config.http_post_format_pressure
  )
  backup.config.http_post2_format_pressure = encodeURIComponent(
    backup.config.http_post2_format_pressure
  )
  backup.config.http_get_format_pressure = encodeURIComponent(
    backup.config.http_get_format_pressure
  )
  backup.config.influxdb2_format_pressure = encodeURIComponent(
    backup.config.influxdb2_format_pressure
  )
  backup.config.mqtt_format_pressure = encodeURIComponent(backup.config.mqtt_format_pressure)

  const s = JSON.stringify(backup, null, 2)
  const name = config.mdns + '.txt'
  download(s, 'text/plain', name)
  global.messageSuccess = 'Backup file created and downloaded as: ' + name
}

function resetFileInput() {
  const fileElement = document.getElementById('upload')
  if (fileElement) {
    fileElement.value = ''
    fileSelected.value = false
  }
}

function restore() {
  const fileElement = document.getElementById('upload')

  if (fileElement.files.length === 0) {
    global.messageError = 'You need to select one file to restore configuration from'
  } else {
    global.disabled = true
    logDebug('BackupView.restore()', 'Selected file: ' + fileElement.files[0].name)
    const reader = new FileReader()
    reader.addEventListener('load', function (e) {
      let text = e.target.result
      try {
        const data = JSON.parse(text)
        if (data.meta.software === 'PressureMon' && data.meta.version === '0.5.0') {
          doRestore(data.config)
          resetFileInput()
        } else {
          global.messageError = 'Unknown format, unable to process'
          resetFileInput()
        }
      } catch (error) {
        logError('BackupView.restore()', 'Failed to parse backup file:', error)
        global.messageError = 'Unable to parse configuration file for PressureMon.'
        resetFileInput()
      }
    })
    reader.readAsText(fileElement.files[0])
  }
}

function download(content, mimeType, filename) {
  const a = document.createElement('a')
  const blob = new Blob([content], { type: mimeType })
  const url = URL.createObjectURL(blob)
  a.setAttribute('href', url)
  a.setAttribute('download', filename)
  a.click()
}

async function doRestore(json) {
  for (var k in json) {
    if (k.endsWith('_format_pressure')) {
      config[k] = decodeURIComponent(json[k])
    } else {
      config[k] = json[k]
    }
  }

  getConfigChanges()
  const ok = await config.saveAll()
  if (ok) {
    global.messageSuccess = 'Configuration has been saved to device'
  }
}
</script>
