/*
 * Pressuremon - Dual License
 *
 * Open Source Use  ->  GNU General Public License v3.0 (GPL v3)
 * Commercial Use   ->  Commercial License Agreement
 *
 * Copyright (c) 2024-2026 Magnus
 *
 * For full license terms and conditions, see LICENSE and LICENSE_COMMERCIAL files.
 * Source: https://github.com/mp-se/pressuremon and https://github.com/mp-se/pressuremon-ui
 */
import { defineStore } from 'pinia'
import {
  logInfo,
  logError,
  logDebug,
  sharedHttpClient as http
} from '@mp-se/espframework-ui-components'

export const useGlobalStore = defineStore('global', {
  state: () => {
    return {
      id: '',
      platform: '',

      board: '',
      app_ver: '',
      app_build: '',
      firmware_file: '',

      ui: {
        enableVoltageFragment: true,
        enableManualWifiEntry: false,
        enableScanForStrongestAp: true,
        enableGravity: false,
        enablePressure: true,
        enableHttpPostTcpMode: false
      },

      feature: {
        ble: false,
        no_sensors: 0
      },

      initialized: false,
      disabled: false,
      configChanged: false,

      messageError: '',
      messageWarning: '',
      messageSuccess: '',
      messageInfo: ''
    }
  },
  getters: {
    isError() {
      return this.messageError != '' ? true : false
    },
    isWarning() {
      return this.messageWarning != '' ? true : false
    },
    isSuccess() {
      return this.messageSuccess != '' ? true : false
    },
    isInfo() {
      return this.messageInfo != '' ? true : false
    },
    uiVersion() {
      logDebug('globalStore.uiVersion()', import.meta.env.VITE_APP_VERSION)
      return import.meta.env.VITE_APP_VERSION
    },
    uiBuild() {
      logDebug('globalStore.uiBuild()', import.meta.env.VITE_APP_BUILD)
      return import.meta.env.VITE_APP_BUILD
    }
  },
  actions: {
    clearMessages() {
      this.messageError = ''
      this.messageWarning = ''
      this.messageSuccess = ''
      this.messageInfo = ''
    },
    async load() {
      logInfo('globalStore.load()', 'Fetching /api/feature')

      try {
        const json = await http.getJson('api/feature', { timeout: this.fetchTimeout })
        logDebug('globalStore.load()', json)

        this.board = json.board.toUpperCase()
        this.app_ver = json.app_ver
        this.app_build = json.app_build
        this.platform = json.platform.toUpperCase()
        this.firmware_file = json.firmware_file.toLowerCase()

        this.feature.ble = json.ble
        this.feature.no_sensors = json.no_sensors

        logInfo('globalStore.load()', 'Fetching /api/feature completed')
        return true
      } catch (err) {
        logError('globalStore.load()', err)
        return false
      }
    }
  }
})
