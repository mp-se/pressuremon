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
#ifndef SRC_BLE_PRESSUREMON_HPP_
#define SRC_BLE_PRESSUREMON_HPP_

#if defined(ENABLE_BLE) && defined(PRESSUREMON)

#include <NimBLEBeacon.h>
#include <NimBLEDevice.h>

class BleSender {
 private:
  BLEServer* _server = nullptr;
  BLEAdvertising* _advertising = nullptr;
  BLEService* _service = nullptr;
  BLECharacteristic* _characteristic = nullptr;
  BLEUUID _uuid;
  bool _initFlag = false;
  int _beaconTime = 1000;

  void dumpPayload(const char* payload, int len);

 public:
  BleSender() {}

  void init();

  // Beacons
  void sendCustomBeaconData(float battery, float tempC, float pressurePsi,
                            float pressurePsi1);
};

#endif  // ENABLE_BLE && PRESSUREMON

#endif  // SRC_BLE_PRESSUREMON_HPP_
