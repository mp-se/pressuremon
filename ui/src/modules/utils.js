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
import i18n from '@/modules/i18n'

export const httpHeaderOptions = ref([
  { label: 'JSON data', value: 'Content-Type: application/json' },
  { label: 'Form data', value: 'Content-Type: x-www-form-urlencoded' },
  { label: 'Authorization', value: 'Authorization: Basic {enter token here}' },
  { label: 'No Cache', value: 'Pragma: no-cache' },
  { label: 'User agent', value: 'User-Agent: pressuremon' }
])

export const httpPostUrlOptions = ref([
  {
    label: '-- none --',
    value: ''
  }
  /*
  {
    label: 'Brewfather ispindel',
    value: 'http://log.brewfather.net/ispindel?id=<yourid>'
  },
  {
    label: 'Brewfather stream',
    value: 'http://log.brewfather.net/stream?id=<yourid>'
  },
  {
    label: 'UBI dots',
    value: 'http://industrial.api.ubidots.com/api/v1.6/devices/<devicename>/?token=<api-token>'
  },
  {
    label: 'UBI dots secure',
    value: 'https://industrial.api.ubidots.com/api/v1.6/devices/<devicename>/?token=<api-token>'
  },
  {
    label: 'Brewersfriend (P)',
    value: 'http://log.brewersfriend.com/ispindel/[API KEY]'
  },
  {
    label: 'Brewersfriend (SG)',
    value: 'http://log.brewersfriend.com/ispindel_sg/[API KEY]'
  },
  { label: 'Brewspy', value: 'http://brew-spy.com/api/ispindel' },
  { label: 'Thingsspeak', value: 'http://api.thingspeak.com/update.json' },
  { label: 'Blynk', value: 'http://blynk.cloud/external/api/batch/update' },
  { label: 'Bierdot bricks', value: 'https://brewbricks.com/api/iot/v1' }
*/
])

export const httpGetUrlOptions = ref([{ label: '-blank-', value: '' }])

// Translates a firmware ESPFWK_WEB_ERR message code, falling back to the
// device's raw (English) message text for codes not yet in locales/*.json.
export function resolveMessage(code, fallbackText) {
  const { t, te } = i18n.global
  return code && te(`messages.${code}`) ? t(`messages.${code}`) : fallbackText
}
