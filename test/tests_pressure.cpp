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
#include <AUnit.h>

#include <pressure.hpp>
#include <config_pressuremon.hpp>
#include <Wire.h>

extern PressuremonConfig myConfig;
extern PressureSensor myPressureSensor;

test(pressure_connectSensor) {
  myConfig.setPressureSensorType(SensorXidibeiXDB401_I2C_KPa_200, 0);
  Wire.begin(PIN_SDA, PIN_SCL);

  myPressureSensor.setup(0, &Wire);
  assertEqual(myPressureSensor.isActive(), true);
  assertEqual(myPressureSensor.read(), true);
}

// EOF
