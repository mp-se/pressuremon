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
#ifndef SRC_PRESSURE_XIDIBEI_HPP_
#define SRC_PRESSURE_XIDIBEI_HPP_

#if defined(PRESSUREMON)

#include <XIDIBEI.hpp>
#include <memory>
#include <pressure.hpp>

class XIDIBEIPressureSensor : public PressureSensorInterface {
 private:
  std::unique_ptr<XIDIBEI> _xidibeiSensor;
  uint8_t _idx;
  float _pressureCorrection = 0, _temperatureCorrection = 0, _maxPressure;
  float _pressure, _temperature;
  bool _sensorActive = false;

 public:
  explicit XIDIBEIPressureSensor(PressureConfigInterface* pressureConfig)
      : PressureSensorInterface(pressureConfig) {}

  bool setup(float maxPressure, TwoWire* wire, SoftWire* softWire, uint8_t idx);
  bool read(bool validate = true);
  bool isActive() { return _sensorActive; }

  float getPressurePsi(bool doCorrection = true);
  float getTemperatureC();

  void calibrate();

  float getAnalogVoltage() { return NAN; }
};

#endif  // PRESSUREMON

#endif  // SRC_PRESSURE_XIDIBEI_HPP_

// EOF
