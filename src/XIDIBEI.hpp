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
#ifndef SRC_XIDIBEI_HPP_
#define SRC_XIDIBEI_HPP_

#if defined(PRESSUREMON)

#include <Arduino.h>
#include <SoftWire.h>
#include <Wire.h>

#include <memory>

constexpr auto XIDIBEI_I2C_ADDRESS = 0x7F;

class XIDIBEI {
 public:
  // Max pressure (kPA) is the maximum value that the sensor can handle.
  explicit XIDIBEI(uint16_t maxPressure, TwoWire *wire = &Wire,
                   SoftWire *softWire = nullptr);

  bool begin();
  // Pressure is returned in kPA
  // Temperature is in degrees C
  bool read(float &pressure, float &temperature);

 private:
  TwoWire *_wire;
  SoftWire *_softWire;
  uint16_t _maxPressure;
};

#endif  // PRESSUREMON

#endif  // SRC_XIDIBEI_HPP_

// EOF
