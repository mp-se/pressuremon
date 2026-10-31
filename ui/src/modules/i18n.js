/*
 * PressureMon - i18n (copied from GravityMon)
 */
import { createI18n } from 'vue-i18n'
import en from '@/locales/en.json'

// Only English ships embedded in the firmware bundle to keep it small.
// Other languages are optional "packs" fetched at runtime on devices.
const i18n = createI18n({
  legacy: false,
  locale: 'en',
  fallbackLocale: 'en',
  messages: { en }
})

export default i18n
