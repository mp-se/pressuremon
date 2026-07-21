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
import { createRouter, createWebHistory } from 'vue-router'
import { validateCurrentForm } from '@mp-se/espframework-ui-components'
import * as badge from '@/modules/badge'
import { global } from '@/modules/pinia'

import HomeView from '@/views/HomeView.vue'
import DeviceSettingsView from '@/views/DeviceSettingsView.vue'
import DeviceHardwareView from '@/views/DeviceHardwareView.vue'
import DeviceSensorView from '@/views/DeviceSensorView.vue'
import DeviceWifiView from '@/views/DeviceWifiView.vue'
import DeviceWifi2View from '@/views/DeviceWifi2View.vue'
import PushSettingsView from '@/views/PushSettingsView.vue'
import PushHttpPost1View from '@/views/PushHttpPost1View.vue'
import PushHttpPost2View from '@/views/PushHttpPost2View.vue'
import PushHttpGetView from '@/views/PushHttpGetView.vue'
import PushInfluxdbView from '@/views/PushInfluxdbView.vue'
import PushMqttView from '@/views/PushMqttView.vue'
import PushBluetoothView from '@/views/PushBluetoothView.vue'
import PushWifiDirectView from '@/views/PushWifiDirectView.vue'
import AboutView from '@/views/AboutView.vue'
import BackupView from '@/views/BackupView.vue'
import FirmwareView from '@/views/FirmwareView.vue'
import SupportView from '@/views/SupportView.vue'
import SerialView from '@/views/SerialView.vue'
import ToolsView from '@/views/ToolsView.vue'
import NotFoundView from '@/views/NotFoundView.vue'

const routes = [
  {
    path: '/',
    name: 'home',
    component: HomeView
  },
  {
    path: '/device/settings',
    name: 'device-settings',
    component: DeviceSettingsView
  },
  {
    path: '/device/hardware',
    name: 'device-hardware',
    component: DeviceHardwareView
  },
  {
    path: '/device/sensor',
    name: 'device-sensor',
    component: DeviceSensorView
  },
  {
    path: '/device/wifi',
    name: 'device-wifi',
    component: DeviceWifiView
  },
  {
    path: '/device/wifi2',
    name: 'device-wifi2',
    component: DeviceWifi2View
  },
  {
    path: '/other/backup',
    name: 'backup',
    component: BackupView
  },
  {
    path: '/other/firmware',
    name: 'firmware',
    component: FirmwareView
  },
  {
    path: '/push/settings',
    name: 'push-settings',
    component: PushSettingsView
  },
  {
    path: '/push/wifi-direct',
    name: 'push-wifi-direct',
    component: PushWifiDirectView
  },
  {
    path: '/push/http-post1',
    name: 'push-http-post1',
    component: PushHttpPost1View
  },
  {
    path: '/push/http-post2',
    name: 'push-http-post2',
    component: PushHttpPost2View
  },
  {
    path: '/push/http-get',
    name: 'push-http-get',
    component: PushHttpGetView
  },
  {
    path: '/push/influxdb',
    name: 'push-influxdb',
    component: PushInfluxdbView
  },
  {
    path: '/push/mqtt',
    name: 'push-mqtt',
    component: PushMqttView
  },
  {
    path: '/push/bluetooth',
    name: 'push-bluetooth',
    component: PushBluetoothView
  },
  {
    path: '/other/support',
    name: 'support',
    component: SupportView
  },
  {
    path: '/other/tools',
    name: 'tools',
    component: ToolsView
  },
  {
    path: '/other/serial',
    name: 'serial',
    component: SerialView
  },
  {
    path: '/other/about',
    name: 'about',
    component: AboutView
  },
  {
    path: '/:catchAll(.*)',
    name: '404',
    component: NotFoundView
  }
]

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: routes
})

export default router

router.beforeEach(() => {
  if (global.disabled) return false

  if (!validateCurrentForm()) return false

  global.clearMessages()
  return true
})

const items = ref([
  {
    label: 'Home',
    icon: 'IconHome',
    path: '/',
    subs: []
  },
  {
    label: 'Device',
    icon: 'IconCpu',
    path: '/device',
    badge: badge.deviceBadge,
    subs: [
      {
        label: 'Settings',
        badge: badge.deviceSettingBadge,
        path: '/device/settings'
      },
      {
        label: 'Hardware',
        badge: badge.deviceHardwareBadge,
        path: '/device/hardware'
      },
      {
        label: 'Sensor',
        badge: badge.deviceSensorBadge,
        path: '/device/sensor'
      },
      {
        label: 'Wifi',
        badge: badge.deviceWifiBadge,
        path: '/device/wifi'
      }
    ]
  },
  {
    label: 'Push targets',
    icon: 'IconCloudUpArrow',
    path: '/push',
    badge: badge.pushBadge,
    subs: [
      {
        label: 'Settings',
        badge: badge.pushSettingBadge,
        path: '/push/settings'
      },
      {
        label: 'WiFi Direct',
        badge: badge.pushWifiDirectBadge,
        path: '/push/wifi-direct'
      },
      {
        label: 'HTTP Post 1',
        badge: badge.pushHttpPost1Badge,
        path: '/push/http-post1'
      },
      {
        label: 'HTTP Post 2',
        badge: badge.pushHttpPost2Badge,
        path: '/push/http-post2'
      },
      {
        label: 'HTTP Get',
        badge: badge.pushHttpGetBadge,
        path: '/push/http-get'
      },
      {
        label: 'Influxdb v2',
        badge: badge.pushInfluxdb2Badge,
        path: '/push/influxdb'
      },
      {
        label: 'MQTT',
        badge: badge.pushMqttBadge,
        path: '/push/mqtt'
      },
      {
        label: 'Bluetooth',
        badge: badge.pushBluetoothBadge,
        path: '/push/bluetooth'
      }
    ]
  },
  {
    label: 'Other',
    icon: 'IconTools',
    path: '/other',
    subs: [
      {
        label: 'Serial console',
        path: '/other/serial'
      },
      {
        label: 'Backup & Restore',
        path: '/other/backup'
      },
      {
        label: 'Firmware update',
        path: '/other/firmware'
      },
      {
        label: 'Support',
        path: '/other/support'
      },
      {
        label: 'Tools',
        path: '/other/tools'
      },
      {
        label: 'About',
        path: '/other/about'
      }
    ]
  }
])

export { items }
