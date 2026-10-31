import { describe, it, expect } from 'vitest'
import { httpHeaderOptions, httpPostUrlOptions, httpGetUrlOptions } from '@/modules/utils'
import { pressureHttpPostFormatOptions } from '@/modules/pressureFormatOptions'
import { pressureHttpGetFormatOptions } from '@/modules/pressureFormatOptions'
import { pressureInfluxdb2FormatOptions } from '@/modules/pressureFormatOptions'
import { pressureMqttFormatOptions } from '@/modules/pressureFormatOptions'

describe('utils constants', () => {
  it('uses pressuremon as the default user agent', () => {
    const userAgent = httpHeaderOptions.value.find((item) => item.label === 'User agent')
    expect(userAgent.value).toBe('User-Agent: pressuremon')
  })

  it('exports httpHeaderOptions with expected items', () => {
    const labels = httpHeaderOptions.value.map((opt) => opt.label)
    expect(labels).toContain('JSON data')
    expect(labels).toContain('Authorization')
    expect(labels).toContain('User agent')
  })

  it('exports httpPostUrlOptions', () => {
    expect(httpPostUrlOptions.value).toBeTruthy()
    expect(Array.isArray(httpPostUrlOptions.value)).toBe(true)
  })

  it('exports httpGetUrlOptions', () => {
    expect(httpGetUrlOptions.value).toBeTruthy()
    expect(Array.isArray(httpGetUrlOptions.value)).toBe(true)
  })
})

describe('pressureFormatOptions from separate modules', () => {
  it('exports pressure format options from pressureFormatOptions module', () => {
    expect(
      pressureHttpPostFormatOptions.value.some((item) => item.label.includes('Pressuremon'))
    ).toBe(true)
    expect(
      pressureHttpGetFormatOptions.value.some((item) => item.label.includes('Pressuremon'))
    ).toBe(true)
    expect(
      pressureInfluxdb2FormatOptions.value.some((item) => item.label.includes('Pressuremon'))
    ).toBe(true)
    expect(pressureMqttFormatOptions.value.some((item) => item.label.includes('Pressuremon'))).toBe(
      true
    )
  })
})
