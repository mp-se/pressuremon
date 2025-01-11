/*
MIT License

Copyright (c) 2025 Magnus

This code is based on application note from vendor with adaption to Arduino.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include <XIDIBEI.hpp>

XIDIBEI::XIDIBEI(uint16_t maxPressure, TwoWire *wire) {
  _wire = wire;
  _maxPressure = maxPressure;
}

bool XIDIBEI::begin() {
  _wire->begin();
  _wire->beginTransmission(XIDIBEI_I2C_ADDRESS);
  return _wire->endTransmission() ? false : true;
}

bool XIDIBEI::readSensor(float &pressure, float &temperature) {
  pressure = NAN;
  temperature = NAN;

  _wire->beginTransmission(XIDIBEI_I2C_ADDRESS);
  _wire->write(0x30);
  _wire->write(0x0A);  // Accuire pressure and temperature data
  if (_wire->endTransmission() != 0) return false;

  // Wait for conversion to be completed
  uint8_t status;
  uint32_t timeout = millis() + 50;

  do {
    if (millis() > timeout) return false;
    delay(5);

    _wire->beginTransmission(XIDIBEI_I2C_ADDRESS);
    _wire->write(0x30);
    if (_wire->endTransmission() != 0) return false;
    if (_wire->requestFrom(XIDIBEI_I2C_ADDRESS, 1) != 1) return false;

    status = _wire->read();
  } while (
      (status & 0x08));  // Bit 3 should be off for conversion to be completed

  _wire->beginTransmission(XIDIBEI_I2C_ADDRESS);
  _wire->write(0x06);
  if (_wire->endTransmission() != 0) return false;
  if (_wire->requestFrom(XIDIBEI_I2C_ADDRESS, 5) != 5) return false;

  uint8_t pressureData[3], temperatureData[2];

  pressureData[0] = _wire->read();  // High bits
  pressureData[1] = _wire->read();
  pressureData[2] = _wire->read();  // Low bits

  temperatureData[0] = _wire->read();  // High bits
  temperatureData[1] = _wire->read();  // Low bits

  float rawPressure = static_cast<uint32_t>(pressureData[0]) << 16 |
                      static_cast<uint32_t>(pressureData[1]) << 8 |
                      static_cast<uint32_t>(pressureData[2]);
  float rawTemperature = static_cast<uint32_t>(temperatureData[0]) << 8 |
                         static_cast<uint32_t>(temperatureData[1]);

  if (rawPressure > 8388608) {  // Value is negative
    pressure = (rawPressure - 16777216) / 8388608;
  } else {
    pressure = rawPressure / 8388608;
  }

  pressure = pressure * _maxPressure;

  if (rawTemperature > 32768) {  // Value is negative
    temperature = (rawTemperature - 65536) / 256;
  } else {
    temperature = rawTemperature / 256;
  }

  return true;
}

// EOF
