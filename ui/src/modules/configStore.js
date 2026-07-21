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
import { global, saveConfigState, getConfigChanges } from '@/modules/pinia'
import {
  logDebug,
  logError,
  logInfo,
  tempToC,
  tempToF,
  roundVal,
  sharedHttpClient as http
} from '@mp-se/espframework-ui-components'

export const useConfigStore = defineStore('config', {
  state: () => {
    return {
      // Device
      id: '',
      mdns: '',
      temp_unit: '',
      // Hardware
      ota_url: '',
      voltage_factor: 0,
      voltage_config: 0,
      gyro_temp: false,
      battery_saving: false,
      battery_type: 0,
      tempsensor_resolution: 0,
      temp_adjustment_value: 0, // C or F
      voltage_pin: 0,
      sensor_type: 0,
      sensor1_type: 0,
      pressure_adjustment: 0.0,
      pressure1_adjustment: 0.0,
      custom_min_voltage: 0,
      custom_max_voltage: 0,
      custom_min_pressure: 0,
      custom_max_pressure: 0,
      custom1_min_voltage: 0,
      custom1_max_voltage: 0,
      custom1_min_pressure: 0,
      custom1_max_pressure: 0,
      pressure_unit: 'psi',
      // Wifi
      wifi_portal_timeout: 0,
      wifi_connect_timeout: 0,
      wifi_ssid: '',
      wifi_ssid2: '',
      wifi_pass: '',
      wifi_pass2: '',
      wifi_direct_ssid: '',
      wifi_direct_pass: '',
      use_wifi_direct: false,
      wifi_scan_ap: false,
      // Push - Generic
      token: '',
      token2: '',
      sleep_interval: 0,
      push_timeout: 0,
      // Push - Http Post 1
      http_post_target: '',
      http_post_header1: '',
      http_post_header2: '',
      http_post_int: 0,
      http_post_format_pressure: '',
      http_post_format_gravity: '',
      // Push - Http Post 2
      http_post2_target: '',
      http_post2_header1: '',
      http_post2_header2: '',
      http_post2_int: 0,
      http_post2_format_pressure: '',
      http_post2_format_gravity: '',
      // Push - Http Get
      http_get_target: '',
      http_get_header1: '',
      http_get_header2: '',
      http_get_int: 0,
      http_get_format_pressure: '',
      http_get_format_gravity: '',
      // Push - Influx
      influxdb2_target: '',
      influxdb2_org: '',
      influxdb2_bucket: '',
      influxdb2_token: '',
      influxdb2_int: 0,
      influxdb2_format_pressure: '',
      influxdb2_format_gravity: '',
      // Push - MQTT
      mqtt_target: '',
      mqtt_port: '',
      mqtt_user: '',
      mqtt_pass: '',
      mqtt_int: 0,
      mqtt_format_pressure: '',
      mqtt_format_gravity: '',
      // Push BLE
      ble_format: 0,
      // Units
      gravity_unit: 'SG',
      // Gravity formula
      dark_mode: false
    }
  },
  actions: {
    convertTemp() {
      if (this.temp_unit == this.internal_temp_unit) return
      if (this.temp_unit == 'C') this.convertTempToC()
      if (this.temp_unit == 'F') this.convertTempToF()
    },
    convertTempToC() {
      if (this.internal_temp_unit == 'C') return

      this.temp_adjustment_value = roundVal(this.temp_adjustment_value / 1.8, 2)
      this.formula_calibration_temp = roundVal(tempToC(this.formula_calibration_temp), 2)
      this.internal_temp_unit = 'C'
    },
    convertTempToF() {
      if (this.internal_temp_unit == 'F') return

      this.temp_adjustment_value = roundVal(this.temp_adjustment_value * 1.8, 2) // Delta value
      this.formula_calibration_temp = roundVal(tempToF(this.formula_calibration_temp), 2)
      this.internal_temp_unit = 'F'
    },
    toJson() {
      logInfo('configStore.toJSON()')
      const dest = {}

      for (const key in this.$state) {
        if (!key.startsWith('$')) {
          dest[key] = this[key]
        }
      }

      logInfo('configStore.toJSON()', dest)
      return JSON.stringify(dest, null, 2)
    },
    async load() {
      global.disabled = true
      logInfo('configStore.load()', 'Fetching /api/config')

      try {
        const json = await http.getJson('api/config')
        logDebug('configStore.load()', json)

        global.disabled = false
        this.id = json.id
        // Device
        this.mdns = json.mdns
        this.temp_unit = json.temp_unit
        // Hardware
        this.ota_url = json.ota_url
        this.voltage_factor = json.voltage_factor
        this.voltage_config = json.voltage_config
        this.battery_saving = json.battery_saving
        this.battery_type = json.battery_type
        this.sensor_type = json.sensor_type
        this.sensor1_type = json.sensor1_type
        this.pressure_adjustment = json.pressure_adjustment
        this.pressure1_adjustment = json.pressure1_adjustment
        this.custom_min_voltage = json.custom_min_voltage
        this.custom_max_voltage = json.custom_max_voltage
        this.custom_min_pressure = json.custom_min_pressure
        this.custom_max_pressure = json.custom_max_pressure
        this.custom1_min_voltage = json.custom1_min_voltage
        this.custom1_max_voltage = json.custom1_max_voltage
        this.custom1_min_pressure = json.custom1_min_pressure
        this.custom1_max_pressure = json.custom1_max_pressure
        this.pressure_unit = json.pressure_unit
        this.temp_adjustment_value = json.temp_adjustment_value
        this.tempsensor_resolution = json.tempsensor_resolution
        this.wifi_portal_timeout = json.wifi_portal_timeout
        this.wifi_connect_timeout = json.wifi_connect_timeout
        this.wifi_ssid = json.wifi_ssid
        this.wifi_ssid2 = json.wifi_ssid2
        this.wifi_pass = json.wifi_pass
        this.wifi_pass2 = json.wifi_pass2
        this.wifi_direct_ssid = json.wifi_direct_ssid
        this.wifi_direct_pass = json.wifi_direct_pass
        this.use_wifi_direct = json.use_wifi_direct
        this.wifi_scan_ap = json.wifi_scan_ap
        // Push - Generic
        this.token = json.token
        this.token2 = json.token2
        this.sleep_interval = json.sleep_interval
        this.push_timeout = json.push_timeout
        // Push - Http Post 1
        this.http_post_target = json.http_post_target
        this.http_post_header1 = json.http_post_header1
        this.http_post_header2 = json.http_post_header2
        this.http_post_int = json.http_post_int
        // this.http_post_format_pressure = json.http_post_format_pressure
        // Push - Http Post 2
        this.http_post2_target = json.http_post2_target
        this.http_post2_header1 = json.http_post2_header1
        this.http_post2_header2 = json.http_post2_header2
        this.http_post2_int = json.http_post2_int
        // this.http_post2_format_pressure = json.http_post2_format_pressure
        // Push - Http Get
        this.http_get_target = json.http_get_target
        this.http_get_header1 = json.http_get_header1
        this.http_get_header2 = json.http_get_header2
        this.http_get_int = json.http_get_int
        // this.http_get_format_pressure = json.http_get_format_pressure
        // Push - Influx
        this.influxdb2_target = json.influxdb2_target
        this.influxdb2_org = json.influxdb2_org
        this.influxdb2_bucket = json.influxdb2_bucket
        this.influxdb2_token = json.influxdb2_token
        this.influxdb2_int = json.influxdb2_int
        // this.influxdb2_format_pressure = json.influxdb2_format_pressure
        // Push - MQTT
        this.mqtt_target = json.mqtt_target
        this.mqtt_port = json.mqtt_port
        this.mqtt_user = json.mqtt_user
        this.mqtt_pass = json.mqtt_pass
        this.mqtt_int = json.mqtt_int
        // this.mqtt_format_pressure = json.mqtt_format_pressure
        // Push BLE
        this.ble_format = json.ble_format
        this.dark_mode = json.dark_mode

        this.internal_temp_unit = 'C'
        this.convertTemp()
        return true
      } catch (err) {
        logError('configStore.load()', err)
        return false
      } finally {
        global.disabled = false
      }
    },
    async loadFormat() {
      global.disabled = true
      logInfo('configStore.loadFormat()', 'Fetching /api/format')
      try {
        const json = await http.getJson('api/format')
        logDebug('configStore.loadFormat()', json)
        global.disabled = false
        this.http_post_format_pressure = decodeURIComponent(json.http_post_format_pressure)
        this.http_post2_format_pressure = decodeURIComponent(json.http_post2_format_pressure)
        this.http_get_format_pressure = decodeURIComponent(json.http_get_format_pressure)
        this.influxdb2_format_pressure = decodeURIComponent(json.influxdb2_format_pressure)
        this.mqtt_format_pressure = decodeURIComponent(json.mqtt_format_pressure)

        if (json.http_post_format_gravity)
          this.http_post_format_gravity = decodeURIComponent(json.http_post_format_gravity)
        if (json.http_post2_format_gravity)
          this.http_post2_format_gravity = decodeURIComponent(json.http_post2_format_gravity)
        if (json.http_get_format_gravity)
          this.http_get_format_gravity = decodeURIComponent(json.http_get_format_gravity)
        if (json.influxdb2_format_gravity)
          this.influxdb2_format_gravity = decodeURIComponent(json.influxdb2_format_gravity)
        if (json.mqtt_format_gravity)
          this.mqtt_format_gravity = decodeURIComponent(json.mqtt_format_gravity)

        // Add linebreaks so the editor shows the data correctly
        this.mqtt_format_pressure = this.mqtt_format_pressure.replaceAll('|', '|\n')
        if (this.mqtt_format_gravity)
          this.mqtt_format_gravity = this.mqtt_format_gravity.replaceAll('|', '|\n')
        return true
      } catch (err) {
        global.disabled = false
        logError('configStore.loadFormat()', err)
        return false
      }
    },
    async sendConfig() {
      global.disabled = true
      logInfo('configStore.sendConfig()', 'Sending /api/config')

      this.convertTempToC() // Device use C internally

      const data = getConfigChanges()
      delete data.http_post_format_pressure
      delete data.http_post2_format_pressure
      delete data.http_get_format_pressure
      delete data.influxdb2_format_pressure
      delete data.mqtt_format_pressure
      delete data.http_post_format_gravity
      delete data.http_post2_format_gravity
      delete data.http_get_format_gravity
      delete data.influxdb2_format_gravity
      delete data.mqtt_format_gravity
      logDebug('configStore.sendConfig()', data)

      if (JSON.stringify(data).length == 2) {
        logInfo('configStore.sendConfig()', 'No config data to store, skipping step')
        global.disabled = false
        this.convertTemp()
        return true
      }

      try {
        await http.postJson('api/config', data)
        global.disabled = false
        logInfo('configStore.sendConfig()', 'Sending /api/config completed')
        this.convertTemp()
        return true
      } catch (err) {
        logError('configStore.sendConfig()', err)
        this.convertTemp()
        global.disabled = false
        return false
      }
    },
    async sendFormat() {
      global.disabled = true
      logInfo('configStore.sendFormat()', 'Sending /api/format')

      const data2 = getConfigChanges()
      let data = {}
      let cnt = 0

      logDebug('configStore.sendFormat()', data2)
      try {
        data =
          data2.http_post_format_pressure !== undefined
            ? { http_post_format_pressure: encodeURIComponent(data2.http_post_format_pressure) }
            : {}
        if (await this.sendOneFormat(data)) cnt += 1

        data =
          data2.http_post2_format_pressure !== undefined
            ? { http_post2_format_pressure: encodeURIComponent(data2.http_post2_format_pressure) }
            : {}
        if (await this.sendOneFormat(data)) cnt += 1

        data =
          data2.http_get_format_pressure !== undefined
            ? { http_get_format_pressure: encodeURIComponent(data2.http_get_format_pressure) }
            : {}
        if (await this.sendOneFormat(data)) cnt += 1

        data =
          data2.influxdb2_format_pressure !== undefined
            ? { influxdb2_format_pressure: encodeURIComponent(data2.influxdb2_format_pressure) }
            : {}
        if (await this.sendOneFormat(data)) cnt += 1

        if (data2.mqtt_format_pressure !== undefined) {
          data2.mqtt_format_pressure = data2.mqtt_format_pressure.replaceAll('\n', '')
          data2.mqtt_format_pressure = data2.mqtt_format_pressure.replaceAll('\r', '')
        }

        data =
          data2.mqtt_format_pressure !== undefined
            ? { mqtt_format_pressure: encodeURIComponent(data2.mqtt_format_pressure) }
            : {}
        if (await this.sendOneFormat(data)) cnt += 1

        data =
          data2.http_post_format_gravity !== undefined
            ? { http_post_format_gravity: encodeURIComponent(data2.http_post_format_gravity) }
            : {}
        if (await this.sendOneFormat(data)) cnt += 1

        data =
          data2.http_post2_format_gravity !== undefined
            ? { http_post2_format_gravity: encodeURIComponent(data2.http_post2_format_gravity) }
            : {}
        if (await this.sendOneFormat(data)) cnt += 1

        data =
          data2.http_get_format_gravity !== undefined
            ? { http_get_format_gravity: encodeURIComponent(data2.http_get_format_gravity) }
            : {}
        if (await this.sendOneFormat(data)) cnt += 1

        data =
          data2.influxdb2_format_gravity !== undefined
            ? { influxdb2_format_gravity: encodeURIComponent(data2.influxdb2_format_gravity) }
            : {}
        if (await this.sendOneFormat(data)) cnt += 1

        if (data2.mqtt_format_gravity !== undefined) {
          data2.mqtt_format_gravity = data2.mqtt_format_gravity.replaceAll('\n', '')
          data2.mqtt_format_gravity = data2.mqtt_format_gravity.replaceAll('\r', '')
        }

        data =
          data2.mqtt_format_gravity !== undefined
            ? { mqtt_format_gravity: encodeURIComponent(data2.mqtt_format_gravity) }
            : {}
        if (await this.sendOneFormat(data)) cnt += 1

        return cnt == 10
      } finally {
        // ensure disabled is cleared by callers as appropriate
        global.disabled = false
      }
    },
    async sendOneFormat(data) {
      logInfo('configStore.sendOneFormat()', 'Sending /api/format', data)

      if (JSON.stringify(data).length == 2) {
        logInfo('configStore.sendOneFormat()', 'No format data to store, skipping step')
        return true
      }

      try {
        await http.postJson('api/format', data)
        global.disabled = false
        logInfo('configStore.sendOneFormat()', 'Sending /api/format completed')
        return true
      } catch (err) {
        logError('configStore.sendOneFormat()', err)
        return false
      }
    },
    async sendPushTest(data) {
      global.disabled = true
      logInfo('configStore.sendPushTest()', 'Sending /api/push')
      try {
        await http.postJson('api/push', data)
        return true
      } catch (err) {
        logError('configStore.sendPushTest()', err)
        return false
      } finally {
        global.disabled = false
      }
    },
    async runPushTest(data) {
      global.disabled = true
      logInfo('configStore.runPushTest()', 'Starting push test')

      try {
        const pushStarted = await this.sendPushTest(data)
        if (!pushStarted) {
          global.messageError = 'Failed to start push test'
          return false
        }

        // Poll for test completion
        const result = await (async () => {
          while (true) {
            const statusRes = await this.getPushTestStatus()
            if (!statusRes.success) {
              global.messageError = 'Failed to get push test status'
              return false
            }

            const d = statusRes.data
            if (d.status) {
              // still running
              await new Promise((r) => setTimeout(r, 2000))
              continue
            }

            if (!d.success) {
              global.messageError = 'Test failed with error code (' + d.push_return_code + ')'
              return true
            } else {
              if (!d.push_enabled) {
                global.messageWarning = 'No endpoint is defined for this target. Cannot run test.'
              } else if (!d.success && d.push_return_code > 0) {
                global.messageError =
                  'Test failed with error code (' + http.getErrorString(d.push_return_code) + ')'
              } else if (!d.success && d.push_return_code == 0) {
                global.messageError =
                  'Test not started. Might be blocked due to skip SSL flag enabled on esp8266'
              } else {
                global.messageSuccess = 'Test was successful'
              }
              return true
            }
          }
        })()

        return result
      } catch (error) {
        logError('configStore.runPushTest()', error)
        global.messageError = 'Push test failed unexpectedly'
        return false
      } finally {
        global.disabled = false
      }
    },

    async sendFilesystemRequest(data, callback) {
      global.disabled = true
      logInfo('configStore.sendFilesystemRequest()', 'Sending filesystem request', data)

      try {
        const res = await http.filesystemRequest(data)
        const success = !!res?.success
        const text = res?.text ?? ''

        if (callback) callback(success, text)
        return { success, text }
      } catch (err) {
        logError('configStore.sendFilesystemRequest()', err)
        if (callback) callback(false, '')
        return { success: false, text: '' }
      } finally {
        global.disabled = false
      }
    },

    async setSleepMode(flag) {
      try {
        logInfo('configStore.setSleepMode()', 'Sending /api/sleepmode')
        const response = await http.request('api/sleepmode', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ sleep_mode: flag })
        })
        if (!response.ok) {
          // Avoid throwing exceptions on non-OK responses. Log details and return false.
          const bodyText = await response.text().catch(() => null)
          logError(
            'configStore.setSleepMode()',
            `HTTP ${response.status}: ${response.statusText}` + (bodyText ? ` - ${bodyText}` : '')
          )
          return false
        }

        // Try to parse JSON if present, but don't throw on parse errors.
        try {
          await response.json()
        } catch (e) {
          logDebug('configStore.setSleepMode()', 'Response JSON parse failed', e)
        }
        logInfo('configStore.setSleepMode()', 'Sending /api/sleepmode completed')
        return true
      } catch (err) {
        logError('configStore.setSleepMode()', err)
        return false
      }
    },
    async restart() {
      global.clearMessages()
      global.disabled = true
      try {
        const res = await http.restart(this.mdns, { redirectDelayMs: 8000 })
        if (res.success && res.json && res.json.status === true) {
          global.messageSuccess =
            (res.json.message || '') +
            ' Redirecting to http://' +
            this.mdns +
            '.local in 8 seconds.'
          logInfo('configStore.restart()', 'Restart requested, redirect scheduled')
        } else if (res.success && res.json) {
          global.messageError = res.json.message || 'Failed to restart device'
        } else {
          global.messageError = 'Failed to request restart'
        }
      } catch (err) {
        logError('configStore.restart()', err)
        global.messageError = 'Failed to do restart'
      } finally {
        global.disabled = false
      }
    },
    async getPushTestStatus() {
      logInfo('configStore.getPushTest()', 'Fetching /api/push/status')
      try {
        const json = await http.getJson('api/push/status')
        logDebug('configStore.getPushTest()', json)
        logInfo('configStore.getPushTest()', 'Fetching /api/push/status completed')
        return { success: true, data: json }
      } catch (err) {
        logError('configStore.getPushTest()', err)
        return { success: false, data: null }
      }
    },
    async sendWifiScan() {
      global.disabled = true
      logInfo('configStore.sendWifiScan()', 'Sending /api/wifi')
      try {
        await http.request('api/wifi')
        logInfo('configStore.sendWifiScan()', 'Sending /api/wifi completed')
        return true
      } catch (err) {
        logError('configStore.sendWifiScan()', err)
        return false
      }
    },
    async getWifiScanStatus() {
      logInfo('configStore.getWifiScanStatus()', 'Fetching /api/wifi/status')
      try {
        const json = await http.getJson('api/wifi/status')
        logDebug('configStore.getWifiScanStatus()', json)
        logInfo('configStore.getWifiScanStatus()', 'Fetching /api/wifi/status completed')
        return { success: true, data: json }
      } catch (err) {
        logError('configStore.getWifiScanStatus()', err)
        return { success: false, data: null }
      }
    },
    async sendHardwareScan() {
      global.disabled = true
      logInfo('configStore.sendHardwareScan()', 'Sending /api/hardware')
      try {
        await http.request('api/hardware')
        logInfo('configStore.sendHardwareScan()', 'Sending /api/hardware completed')
        return true
      } catch (err) {
        logError('configStore.sendHardwareScan()', err)
        return false
      }
    },
    async getHardwareScanStatus() {
      logInfo('configStore.getHardwareScanStatus()', 'Fetching /api/hardware/status')
      try {
        const json = await http.getJson('api/hardware/status')
        logDebug('configStore.getHardwareScanStatus()', json)
        logInfo('configStore.getHardwareScanStatus()', 'Fetching /api/hardware/status completed')
        return { success: true, data: json }
      } catch (err) {
        logError('configStore.getHardwareScanStatus()', err)
        return { success: false, data: null }
      }
    },
    async saveAll() {
      global.clearMessages()
      global.disabled = true

      try {
        const configSuccess = await this.sendConfig()
        if (!configSuccess) {
          global.messageError = 'Failed to store configuration to device'
          return
        }

        const formatSuccess = await this.sendFormat()
        if (!formatSuccess) {
          global.messageError = 'Failed to store format to device'
          return
        }

        global.messageSuccess = 'Configuration has been saved to device'
        saveConfigState()
      } catch (error) {
        logError('configStore.saveAll()', error)
        global.messageError = 'Failed to save configuration'
      } finally {
        global.disabled = false
      }
    },

    async runWifiScan() {
      global.disabled = true
      logInfo('configStore.runWifiScan()', 'Starting wifi scan')

      try {
        const started = await this.sendWifiScan()
        if (!started) {
          global.messageError = 'Failed to start wifi scan'
          return { success: false }
        }

        while (true) {
          const statusRes = await this.getWifiScanStatus()
          if (!statusRes.success) {
            global.messageError = 'Failed to get wifi scan status'
            return { success: false }
          }

          if (statusRes.data.status) {
            await new Promise((r) => setTimeout(r, 2000))
            continue
          }

          global.disabled = false
          return { success: statusRes.data.success, data: statusRes.data }
        }
      } finally {
        global.disabled = false
      }
    },

    async runHardwareScan() {
      global.disabled = true
      logInfo('configStore.runHardwareScan()', 'Starting hardware scan')

      try {
        const started = await this.sendHardwareScan()
        if (!started) {
          global.messageError = 'Failed to start hardware scan'
          return { success: false }
        }

        while (true) {
          const statusRes = await this.getHardwareScanStatus()
          if (!statusRes.success) {
            global.messageError = 'Failed to get hardware scan status'
            return { success: false }
          }

          if (statusRes.data.status) {
            await new Promise((r) => setTimeout(r, 2000))
            continue
          }

          global.disabled = false
          return { success: statusRes.data.success, data: statusRes.data }
        }
      } finally {
        global.disabled = false
      }
    }
  }
})
