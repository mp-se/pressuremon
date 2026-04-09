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

#if defined(ENABLE_BLE) && defined(PRESSUREMON)

#include <ble_pressuremon.hpp>
#include <log.hpp>
#include <string>

void BleSender::init() {
  if (_initFlag) return;

  BLEDevice::init("pressuremon");
  _advertising = BLEDevice::getAdvertising();

  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN, ESP_PWR_LVL_P9);

  _initFlag = true;
}

void BleSender::sendCustomBeaconData(float battery, float tempC,
                                     float pressurePsi, float pressurePsi1) {
  Log.info(F("Starting custom beacon data transmission" CR));

  _advertising->stop();

  uint16_t p = isnan(pressurePsi) ? 0xffff : pressurePsi * 100;
  uint16_t p1 = isnan(pressurePsi1) ? 0xffff : pressurePsi1 * 100;
  uint16_t t = tempC * 1000;
  uint16_t b = isnan(battery) ? 0xffff : battery * 1000;
  uint32_t chipId = 0;

  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  std::string mf = "";

  mf += static_cast<char>(0x4C);  // Manuf ID (Apple)
  mf += static_cast<char>(0x00);
  mf += static_cast<char>(0x03);  // SubType (standards is 0x02)
  mf += static_cast<char>(0x15);  // SubType Length
  mf += "PRESMON.";
  mf += static_cast<char>(((chipId & 0xFF000000) >> 24));  // Chipid
  mf += static_cast<char>(((chipId & 0xFF0000) >> 16));
  mf += static_cast<char>(((chipId & 0xFF00) >> 8));
  mf += static_cast<char>((chipId & 0xFF));
  mf += static_cast<char>((p >> 8));  // Pressure (pressure*10000)
  mf += static_cast<char>((p & 0xFF));
  mf += static_cast<char>((p1 >> 8));  // Pressure1 (pressure1*10000)
  mf += static_cast<char>((p1 & 0xFF));
  mf += static_cast<char>((b >> 8));  // Battery (batt_v*1000)
  mf += static_cast<char>((b & 0xFF));
  mf += static_cast<char>((t >> 8));  // Temperature (temp_c*1000)
  mf += static_cast<char>((t & 0xFF));
  mf += static_cast<char>(0x00);  // Signal

#if LOG_LEVEL == 6
  dumpPayload(mf.c_str(), mf.length());
#endif

  BLEAdvertisementData advData = BLEAdvertisementData();
  advData.setFlags(0x04);
  advData.setManufacturerData(mf);
  _advertising->setAdvertisementData(advData);

  // _advertising->setAdvertisementType(BLE_GAP_CONN_MODE_NON);
  _advertising->setConnectableMode(BLE_GAP_CONN_MODE_NON);
  _advertising->start();
  delay(_beaconTime);
  _advertising->stop();
}

void BleSender::dumpPayload(const char* p, int len) {
  for (int i = 0; i < len; i++) {
    EspSerial.printf("%X%X ", (*(p + i) & 0xf0) >> 4, (*(p + i) & 0x0f));
  }
  EspSerial.println();
}

#endif  // ENABLE_BLE && PRESSUREMON
