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
import { createApp } from 'vue'
import App from './App.vue'

const app = createApp(App)

import piniaInstance from './modules/pinia.js'
app.use(piniaInstance)

import router from './modules/router.js'
app.use(router)

// Import all components from the ESP Framework UI Components library
import {
  // Bootstrap Components
  BsMessage,
  BsCard,
  BsFileUpload,
  BsProgress,
  BsInputBase,
  BsInputText,
  BsInputReadonly,
  BsSelect,
  BsInputTextArea,
  BsInputNumber,
  BsInputSwitch,
  BsInputRadio,
  BsDropdown,
  BsModal,
  BsModalConfirm,
  BsInputTextAreaFormat,
  BsMenuBar,
  BsFooter,
  // Icon Components
  IconHome,
  IconTools,
  IconGraphUpArrow,
  IconCloudUpArrow,
  IconUpArrow,
  IconCpu,
  IconWifi,
  IconEye,
  IconEyeSlash,
  IconCheckCircle,
  IconXCircle,
  IconExclamationTriangle,
  IconInfoCircle
} from '@mp-se/espframework-ui-components'

// Import local fragments
import AdvancedFilesFragment from '@/fragments/AdvancedFilesFragment.vue'
import EnableCorsFragment from '@/fragments/EnableCorsFragment.vue'
import ListFilesFragment from '@/fragments/ListFilesFragment.vue'
import VoltageFragment from '@/fragments/VoltageFragment.vue'

// Import Bootstrap CSS and JS first, then library CSS to allow overrides
import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap/dist/js/bootstrap.bundle.js'
// import '@mp-se/espframework-ui-components/dist/style.css'

// Register Bootstrap components
app.component('BsMessage', BsMessage)
app.component('BsCard', BsCard)
app.component('BsFileUpload', BsFileUpload)
app.component('BsProgress', BsProgress)
app.component('BsInputBase', BsInputBase)
app.component('BsInputText', BsInputText)
app.component('BsInputReadonly', BsInputReadonly)
app.component('BsSelect', BsSelect)
app.component('BsInputTextArea', BsInputTextArea)
app.component('BsInputNumber', BsInputNumber)
app.component('BsInputSwitch', BsInputSwitch)
app.component('BsInputRadio', BsInputRadio)
app.component('BsDropdown', BsDropdown)
app.component('BsModal', BsModal)
app.component('BsModalConfirm', BsModalConfirm)
app.component('BsInputTextAreaFormat', BsInputTextAreaFormat)
app.component('BsMenuBar', BsMenuBar)
app.component('BsFooter', BsFooter)

// Register Icon components
app.component('IconHome', IconHome)
app.component('IconTools', IconTools)
app.component('IconGraphUpArrow', IconGraphUpArrow)
app.component('IconCloudUpArrow', IconCloudUpArrow)
app.component('IconUpArrow', IconUpArrow)
app.component('IconCpu', IconCpu)
app.component('IconWifi', IconWifi)
app.component('IconEye', IconEye)
app.component('IconEyeSlash', IconEyeSlash)
app.component('IconCheckCircle', IconCheckCircle)
app.component('IconXCircle', IconXCircle)
app.component('IconExclamationTriangle', IconExclamationTriangle)
app.component('IconInfoCircle', IconInfoCircle)

app.component('AdvancedFilesFragment', AdvancedFilesFragment)
app.component('EnableCorsFragment', EnableCorsFragment)
app.component('ListFilesFragment', ListFilesFragment)
app.component('VoltageFragment', VoltageFragment)

app.mount('#app')
