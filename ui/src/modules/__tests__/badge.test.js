import { beforeEach, describe, expect, it, vi } from 'vitest'

const { mockConfig } = vi.hoisted(() => ({
  mockConfig: {
    mdns: '',
    wifi_ssid: '',
    wifi_ssid2: '',
    http_post_target: '',
    http_post2_target: '',
    http_get_target: '',
    influxdb2_target: '',
    mqtt_target: '',
    ble_format: 0,
    use_wifi_direct: false
  }
}))

vi.mock('@/modules/pinia', () => ({
  config: mockConfig
}))

import * as badge from '../badge'

describe('badge.js', () => {
  beforeEach(() => {
    mockConfig.mdns = 'pressuremon-test'
    mockConfig.wifi_ssid = 'ssid-1'
    mockConfig.wifi_ssid2 = ''
    mockConfig.http_post_target = ''
    mockConfig.http_post2_target = ''
    mockConfig.http_get_target = ''
    mockConfig.influxdb2_target = ''
    mockConfig.mqtt_target = ''
    mockConfig.ble_format = 0
    mockConfig.use_wifi_direct = false
  })

  it('deviceMdnsBadge returns 1 when mdns is empty', () => {
    mockConfig.mdns = ''
    expect(badge.deviceMdnsBadge()).toBe(1)
  })

  it('deviceWifi1Badge returns 1 when primary ssid is empty', () => {
    mockConfig.wifi_ssid = ''
    expect(badge.deviceWifi1Badge()).toBe(1)
  })

  it('deviceWifi2Badge returns 1 only when both ssids are empty', () => {
    mockConfig.wifi_ssid = ''
    mockConfig.wifi_ssid2 = ''
    expect(badge.deviceWifi2Badge()).toBe(1)

    mockConfig.wifi_ssid = 'ssid-1'
    expect(badge.deviceWifi2Badge()).toBe(0)
  })

  it('deviceBadge combines mdns and wifi badges', () => {
    mockConfig.mdns = ''
    mockConfig.wifi_ssid = ''
    mockConfig.wifi_ssid2 = ''
    expect(badge.deviceBadge()).toBe(2)
  })

  it('deviceSettingBadge delegates to the mdns badge', () => {
    mockConfig.mdns = ''
    expect(badge.deviceSettingBadge()).toBe(1)
  })

  it('deviceHardwareBadge and sensor badge helpers return zero defaults', () => {
    expect(badge.deviceHardwareBadge()).toBe(0)
    expect(badge.deviceSensorCalibratedBadge()).toBe(0)
    expect(badge.deviceSensorBadge()).toBe(0)
  })

  it('deviceWifiBadge returns zero when at least one wifi target is configured', () => {
    mockConfig.wifi_ssid = 'ssid-1'
    mockConfig.wifi_ssid2 = ''
    expect(badge.deviceWifiBadge()).toBe(0)
  })

  it('push badges require at least one configured target', () => {
    expect(badge.pushBadge()).toBe(6)

    mockConfig.mqtt_target = 'mqtt://broker'
    expect(badge.pushBadge()).toBe(0)
  })

  it('ble_format counts as a push target', () => {
    mockConfig.ble_format = 1
    expect(badge.pushBluetoothBadge()).toBe(0)
    expect(badge.pushHttpPost1Badge()).toBe(0)
  })

  it('wifi direct counts as a push target', () => {
    mockConfig.use_wifi_direct = true
    expect(badge.pushHttpGetBadge()).toBe(0)
  })

  it('pushSettingBadge stays zero and each push badge reports missing targets', () => {
    expect(badge.pushSettingBadge()).toBe(0)
    expect(badge.pushHttpPost1Badge()).toBe(1)
    expect(badge.pushHttpPost2Badge()).toBe(1)
    expect(badge.pushHttpGetBadge()).toBe(1)
    expect(badge.pushInfluxdb2Badge()).toBe(1)
    expect(badge.pushMqttBadge()).toBe(1)
    expect(badge.pushBluetoothBadge()).toBe(1)
  })
})
