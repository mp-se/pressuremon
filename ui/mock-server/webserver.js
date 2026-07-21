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
import { createRequire } from 'module'
const require = createRequire(import.meta.url)
const express = require('express')

const app = express()
const port = 3000

app.use(express.json())

app.use('/', express.static('dist'))

app.listen(port, () => {
  console.log(`Simple web server port ${port}`)
})
