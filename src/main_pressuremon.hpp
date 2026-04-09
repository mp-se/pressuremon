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
#ifndef SRC_MAIN_PRESSUREMON_HPP_
#define SRC_MAIN_PRESSUREMON_HPP_

#if defined(PRESSUREMON)

#include <Arduino.h>

#if defined(LOLIN_S3_MINI)
// Hardware config for ESP32-s3-mini, pressuremon hardware
// ------------------------------------------------------
#define PIN_SDA 35
#define PIN_SCL 36
#define PIN_SDA1 6
#define PIN_SCL1 5
#define PIN_VOLT 2
#define PIN_DS 10
#define PIN_PWR 11
#define PIN_CFG1 TX
#define PIN_CFG2 RX
#define CFG_FILENAMEBIN "firmware32c3mini.bin"

#elif defined(LOLIN_S2_MINI)
// Hardware config for ESP32-s2-mini, pressuremon hardware
// ------------------------------------------------------
#define PIN_SDA 33
#define PIN_SCL 35
#define PIN_SDA1 6
#define PIN_SCL1 4
#define PIN_VOLT 3
#define PIN_DS 12
#define PIN_PWR 11
#define PIN_CFG1 TX
#define PIN_CFG2 RX
#define CFG_FILENAMEBIN "firmware32s2mini.bin"

#elif defined(LOLIN_C3_PICO)
// Hardware config for ESP32-c3-pico, pressuremon hardware
// ------------------------------------------------------------
#define PIN_SDA 8
#define PIN_SCL 10
#define PIN_SDA1 1
#define PIN_SCL1 2
#define PIN_VOLT 3
#define PIN_DS 5
#define PIN_PWR 4
#define PIN_CFG1 TX
#define PIN_CFG2 RX
#define CFG_FILENAMEBIN "firmware32c3pico.bin"

#elif defined(D1_MINI)
// Hardware config for ESP8266-d1-mini, only for unit tests
// --------------------------------------------------------
#define PIN_SDA 8
#define PIN_SCL 10
#define PIN_SDA1 1
#define PIN_SCL1 2
#define PIN_VOLT 3
#define PIN_DS 5
#define PIN_PWR 4
#define PIN_CFG1 TX
#define PIN_CFG2 RX
#define CFG_FILENAMEBIN ""

#endif

#endif  // PRESSUREMON

#endif  // SRC_MAIN_PRESSUREMON_HPP_
