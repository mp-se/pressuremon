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
import { ref } from 'vue'

export const pressureHttpPostFormatOptions = ref([
  {
    label: 'Pressuremon (null measurements)',
    value: encodeURIComponent(
      '{"name":"${mdns}","ID":"${id}","token":"${token}","interval":${sleep-interval},"temperature":${temp-null},"temp_units":"${temp-unit}","pressure":${pressure-null},"pressure1":${pressure1-null},"pressure_unit":"${pressure-unit}","battery":${battery},"RSSI":${rssi}}'
    )
  },
  {
    label: 'Pressuremon (Single)',
    value: encodeURIComponent(
      '{"name":"${mdns}","ID":"${id}","token":"${token}","interval":${sleep-interval},"temperature":${temp},"temp_units":"${temp-unit}","pressure":${pressure},"pressure_unit":"${pressure-unit}","battery":${battery},"RSSI":${rssi}}'
    )
  },
  {
    label: 'Pressuremon (Dual)',
    value: encodeURIComponent(
      '{"name":"${mdns}","ID":"${id}","token":"${token}","interval":${sleep-interval},"temperature":${temp},"temp_units":"${temp-unit}","pressure1":${pressure1},"pressure2":${pressure2},"pressure_unit":"${pressure-unit}","battery":${battery},"RSSI":${rssi}}'
    )
  },
  {
    label: 'Brewblox (Single)',
    value: encodeURIComponent(
      '{"name":"${mdns}","temp":${temp},"aux_temp":0,"ext_temp":0,"temp_unit":"${temp-unit}","pressure":${pressure},"pressure_unit":"${pressure-unit}","battery":${battery}}'
    )
  },
  {
    label: 'Brewblox (Dual)',
    value: encodeURIComponent(
      '{"name":"${mdns}","temp":${temp},"aux_temp":0,"ext_temp":0,"temp_unit":"${temp-unit}","pressure":${pressure1},"aux_pressure":${pressure2},"pressure_unit":"${pressure-unit}","battery":${battery}}'
    )
  }
])

export const pressureHttpGetFormatOptions = ref([
  {
    label: 'Pressuremon (Single)',
    value: encodeURIComponent(
      '?name=${mdns}&id=${id}&token=${token2}&interval=${sleep-interval}&temperature=${temp}&temp-units=${temp-unit}&pressure=${pressure}&pressure-unit=${pressure-unit}&battery=${battery}&rssi=${rssi}'
    )
  },
  {
    label: 'Pressuremon (Dual)',
    value: encodeURIComponent(
      '?name=${mdns}&id=${id}&token=${token2}&interval=${sleep-interval}&temperature=${temp}&temp-units=${temp-unit}&pressure1=${pressure1}&pressure2=${pressure2}&pressure-unit=${pressure-unit}&battery=${battery}&rssi=${rssi}'
    )
  }
])

export const pressureInfluxdb2FormatOptions = ref([
  {
    label: 'Pressuremon (Single)',
    value: encodeURIComponent(
      'measurement,host=${mdns},device=${id},temp-format=${temp-unit},pressure-format=${pressure-unit} pressure=${pressure},temp=${temp},battery=${battery},rssi=${rssi}\n'
    )
  },
  {
    label: 'Pressuremon (Dual)',
    value: encodeURIComponent(
      'measurement,host=${mdns},device=${id},temp-format=${temp-unit},pressure-format=${pressure-unit} pressure1=${pressure1},pressure2=${pressure2},temp=${temp},battery=${battery},rssi=${rssi}\n'
    )
  }
])

export const pressureMqttFormatOptions = ref([
  {
    label: 'Pressuremon / Brewblox',
    value: encodeURIComponent(
      'pressuremon/${mdns}/pressure:${pressure}|pressuremon/${mdns}/temperature:${temp}|pressuremon/${mdns}/temp_units:${temp-unit}|pressuremon/${mdns}/press_units:${pressure-unit}|pressuremon/${mdns}/battery:${battery}|pressuremon/${mdns}/RSSI:${rssi}|'
    )
  },
  {
    label: 'HomeAssistant',
    value: encodeURIComponent(
      'pressuremon/${mdns}/pressure:${pressure}|pressuremon/${mdns}/temperature:${temp}|pressuremon/${mdns}/rssi:${rssi}|pressuremon/${mdns}/battery:${battery}|'
    )
  }
])
