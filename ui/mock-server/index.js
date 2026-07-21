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
/*
 * Project specific mock server
 *
 * (c) 2023-2024 Magnus Persson
 */
import express from 'express'
import cors from 'cors'
import { registerEspFwk } from './espfwk.js'
import { configData, statusData, formatData } from './data.js'

const app = express()
const port = 3000

app.use(cors())
app.use(express.json())

registerEspFwk(app)

var calibrateRunning = false

app.get('/api/calibrate', (req, res) => {
  console.log('GET: /api/calibrate')
  /* 
   * Description:    Initiate the gyro calibration process. 
   * Authentication: Required
   * Limitation:     - 
   * Note:           Use /api/calibrate/status to check for completion
   * Return:         200 OK, 401 Access Denied
   */
  configData.gyro_calibration_data.gx = 1
  setTimeout(() => { calibrateRunning = false }, 5000)
  calibrateRunning = true
  var data = {
    success: true,
    message: "Gyro calibration started..."
  }
  res.type('application/json')
  res.send(data)
})

app.get('/api/ping', (req, res) => {
  console.log('GET: /api/ping')
  /* 
   * Description:    Check for response from device. 
   * Authentication: Required
   * Limitation:     - 
   * Return:         200 OK
   */
  var data = {
    status: true,
  }
  res.type('application/json')
  res.send(data)
})

app.get('/api/calibrate/status', (req, res) => {
  console.log('GET: /api/calibrate/status')
  /* 
   * Description:    Return status of the current gyro calibration process. 
   * Authentication: Required
   * Limitation:     - 
   * Note:           -
   * Return:         200 OK, 401 Access Denied
   */
  var data = {}
  if (calibrateRunning) {
    data = {
      status: calibrateRunning,
      success: false,
      message: "Gyro calibration running..."
    }
  } else {
    data = {
      status: false,
      success: true,
      message: "Gyro calibration completed..."
    }
  }
  res.type('application/json')
  res.send(data)
})

var testRunning = false

app.post('/api/push', (req, res) => {
  console.log('GET: /api/push')
  /* 
   * Description:    Initiate the push test for a defined target
   * Authentication: Required
   * Limitation:     - 
   * Note:           Use /api/test/push/status to check for completion
   * Return:         200 OK, 401 Access Denied, 422 Content Invalid
   * Request body:
     {
       push_format: "http_format|http_format2|http_format3|influxdb2_format|mqtt_format"
     }
   */
  if(!Object.prototype.hasOwnProperty.call(req.body, "push_format")) {
    res.sendStatus(422)
    return
  }  
  testRunning = true
  setTimeout(() => { testRunning = false }, 5000)
  var data = {
    success: true,
    message: "Test scheduled."
  }
  res.type('application/json')
  res.send(data)
})

app.get('/api/push/status', (req, res) => {
  console.log('GET: /api/push/status')
  /* 
   * Description:    Return status of the current gyro calibration process. 
   * Authentication: Required
   * Limitation:     - 
   * Note:           -
   * Return:         200 OK, 401 Access Denied
   */
  var data = {}
  if (testRunning) {
    data = {
      status: testRunning,
      success: false,
      message: "Push test running..."
    }
  } else {
    data = {
      status: false,
      success: true,
      message: "Push test completed...",
      push_return_code: 200,
      push_enabled: true
    }
  }
  res.type('application/json')
  res.send(data)
})

app.get('/api/formula', (req, res) => {
  console.log('GET: /api/formula')
  /* 
   * Description:    Simualate creation of gravity formula. Assume data has been saved via config.
   * Authentication: Required
   * Limitation:     - 
   * Note:           -
   * Return:         200 OK, 401 Access Denied
   */
  setTimeout(() => {
    var data = {
      success: true,
      message: "Hello world",
      gravity_formula: "0.0*tilt^3+0.0*tilt^2+0.0017978*tilt+0.9436"
    }
    res.type('application/json')
    res.send(data)
  }, 2000)
})

app.get('/api/format', (req, res) => {
  console.log('GET: /api/format')
  /* 
   * Description:    Return format data as json document. 
   * Authentication: Required
   * Limitation:     -
   * Note:           -
   * Return:         200 OK, 401 Access Denied
   */
  res.type('application/json')
  res.send(formatData)
})

app.post('/api/format', (req, res) => {
  console.log('POST: /api/format')
  /* 
   * Description:    Update the format data that is in body
   * Authentication: Required
   * Limitation:     - 
   * Note:           See format read for options.
   * Return:         200 OK, 401 Access Denied
   */
  console.log(req.body);
  for (var o in req.body) {
    formatData[o] = req.body[o]
  }
  var data = {
    success: true,
    message: "Format stored",
  }
  res.type('application/json')
  res.send(data)
})

app.post('/api/sleepmode', (req, res) => {
  console.log('POST: /api/sleepmode')
  /* 
   * Description:    Toggle the sleep mode (from index page)
   * Authentication: Required
   * Limitation:     - 
   * Note:           Returns current sleep_mode
   * Return:         200 OK, 401 Access Denied
   * Request body:
     {
       sleep_mode: true|false
     }
   */
  statusData.sleep_mode = req.body.sleep_mode
  var data = { 
    sleep_mode: req.body.sleep_mode 
  }
  res.type('application/json')
  res.send(data)
})

app.listen(port, () => {
  console.log(`Pressuremon API simulator port http://localhost:${port}/`)
})