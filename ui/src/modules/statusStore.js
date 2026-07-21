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
import { global } from '@/modules/pinia'
import { sharedHttpClient as http } from '@mp-se/espframework-ui-components'
import { logDebug, logError, logInfo } from '@mp-se/espframework-ui-components'

export const useStatusStore = defineStore('status', {
  state: () => {
    return {
      id: '',

      pressure: 10,
      pressure1: 11,
      pressure_unit: 'PSI',
      temp: 20.2,
      temp1: 20.5,
      temp_unit: 'C',

      sleep_interval: 0,
      battery: 0,
      sleep_mode: false,
      rssi: 0,
      mdns: '',
      wifi_ssid: '',
      ip: '',
      total_heap: 0,
      free_heap: 0,
      self_check: {
        sensor_configured: true,
        sensor_connected: true,
        temp_connected: true,
        battery_level: true,
        push_targets: true
      },
      wifi_setup: false,
      max_sensors: 1,
      adc_found: false,
      connected: true
    }
  },
  getters: {
    isPsi() {
      return this.pressure_unit === 'PSI'
    },
    isBar() {
      return this.pressure_unit === 'Bar'
    },
    isKPa() {
      return this.pressure_unit === 'kPa'
    }
  },
  actions: {
    async authAsync() {
      logInfo('statusStore.authAsync()', 'Authenticating via framework http.auth()')
      try {
        const ok = await http.auth()
        if (!ok) {
          logError('statusStore.authAsync()', 'http.auth() returned false')
          return { success: false, data: null }
        }

        // Try to expose the token from the shared client or fallback to global.id
        const token = http.token || global.id || null
        return { success: true, data: { token } }
      } catch (err) {
        logError('statusStore.authAsync()', err)
        return { success: false, data: null }
      }
    },
    async load() {
      logInfo('statusStore.load()', 'Fetching /api/status')

      try {
        const response = await http.request('api/status', { timeout: global.fetchTimeout })

        const json = await response.json()
        logDebug('statusStore.load()', json)

        this.id = json.id
        this.pressure = json.pressure
        this.pressure1 = json.pressure1 // Todo fix NaN values
        this.pressure_unit = json.pressure_unit
        this.temp = json.temp
        this.temp_unit = json.temp_unit

        this.sleep_mode = json.sleep_mode
        this.battery = json.battery
        this.rssi = json.rssi
        this.mdns = json.mdns
        this.wifi_ssid = json.wifi_ssid
        this.ip = json.ip

        this.self_check.sensor_connected = json.self_check.sensor_connected
        this.self_check.sensor_configured = json.self_check.sensor_configured
        this.self_check.battery_level = json.self_check.battery_level
        this.self_check.push_targets = json.self_check.push_targets

        this.total_heap = json.total_heap
        this.free_heap = json.free_heap
        this.wifi_setup = json.wifi_setup
        this.max_sensors = json.max_sensors
        this.adc_found = json.adc_found

        this.total_heap = Math.round(this.total_heap / 1024).toFixed(0)
        this.free_heap = Math.round(this.free_heap / 1024).toFixed(0)

        if (this.pressure !== undefined)
          this.pressure = (Math.round(this.pressure * 100) / 100).toFixed(2)

        if (this.pressure1 !== undefined)
          this.pressure1 = (Math.round(this.pressure1 * 100) / 100).toFixed(2)

        if (this.temp !== undefined) this.temp = (Math.round(this.temp * 100) / 100).toFixed(2)

        this.battery = (Math.round(this.battery * 100) / 100).toFixed(2)

        logInfo('statusStore.load()', 'Fetching /api/status completed')
        return true
      } catch (err) {
        logError('statusStore.load()', err)
        return false
      }
    }
  }
})
