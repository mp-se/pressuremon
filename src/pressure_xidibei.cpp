/*
 * PressureMon
 * Copyright (c) 2021-2026 Magnus
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Alternatively, this software may be used under the terms of a
 * commercial license. See LICENSE_COMMERCIAL for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#if defined(PRESSUREMON)

#include <helper.hpp>
#include <pressure_xidibei.hpp>

constexpr auto XIDIBEI_IIC_CALIBRATION_COUNT = 5;

bool XIDIBEIPressureSensor::setup(float maxPressure, TwoWire *wire,
                                  SoftWire *softWire, uint8_t idx) {
  _pressureCorrection = _pressureConfig->getPressureSensorCorrection(idx);
  _temperatureCorrection = _pressureConfig->getTemperatureSensorCorrection(idx);
  _maxPressure = maxPressure;
  _idx = idx;
  _xidibeiSensor.reset(new XIDIBEI(_maxPressure, wire, softWire));
  _sensorActive = _xidibeiSensor->begin();
  Log.notice(
      F("PRES: XIDIBEI sensor initialized = %s, max pressure = %F, pressure "
        "correction = %F, temperature correction = %F (%d)" CR),
      _sensorActive ? "true" : "false", _maxPressure, _pressureCorrection,
      _temperatureCorrection, _idx);
  return _sensorActive;
}

void XIDIBEIPressureSensor::calibrate() {
  Log.notice(F("PRES: Starting auto calibration (%d)." CR), _idx);
  float zero = 0;

  for (int i = 0; i < XIDIBEI_IIC_CALIBRATION_COUNT; i++) {
    read(false);
    float f = getPressurePsi(false);
    zero += f;
    Log.notice(F("PRES: Step %d, Pressure = %F, sum %F (%d)." CR), i + 1, f,
               zero, _idx);
    delay(500);
  }

  Log.notice(F("PRES: Measured difference, ave %F, sum %F (%d)." CR),
             zero / XIDIBEI_IIC_CALIBRATION_COUNT, zero, _idx);
  _pressureConfig->setPressureSensorCorrection(
      -(zero / XIDIBEI_IIC_CALIBRATION_COUNT), _idx);
  _pressureConfig->saveFile();
  _pressureCorrection = _pressureConfig->getPressureSensorCorrection(_idx);
}

float XIDIBEIPressureSensor::getTemperatureC() {
  return _temperature + _temperatureCorrection;
}

float XIDIBEIPressureSensor::getPressurePsi(bool doCorrection) {
  if (doCorrection) return _pressure + _pressureCorrection;
  return _pressure;
}

bool XIDIBEIPressureSensor::read(bool validate) {
  float pressure;

  // Returns temperature in C and pressure in kPa
  bool b = _xidibeiSensor->read(pressure, _temperature);
  _pressure = convertPaPressureToPsi(pressure * 1000);

  if (validate) {
    if (_pressure > _maxPressure) {
      Log.warning(
          F("PRES: Read pressure is invalid and out of range %F (%d)." CR),
          _pressure, _idx);
      _pressure = NAN;
      return false;
    }
  }

  return b;
}

#endif  // PRESSUREMON

// EOF
