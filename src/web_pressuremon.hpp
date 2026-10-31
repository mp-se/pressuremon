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
#ifndef SRC_WEB_PRESSUREMON_HPP_
#define SRC_WEB_PRESSUREMON_HPP_

#if defined(PRESSUREMON)

#include <battery.hpp>
#include <config_pressuremon.hpp>
#include <pushtarget.hpp>
#include <templating.hpp>
#include <web_brewing.hpp>

class PressuremonWebServer : public BrewingWebServer {
 private:
  PressuremonConfig *_pressConfig = nullptr;

  void doTaskSensorCalibration();
  void doTaskPushTestSetup(TemplatingEngine &engine, BrewingPush &push);
  void doTaskHardwareScanning(JsonObject &obj);

  void doWebStatus(JsonObject &obj);
  void doWebFeature(JsonObject &obj);
  void doWebConfigWrite();
  void doWebCalibrateStatus(JsonObject &obj);

 public:
  explicit PressuremonWebServer(PressuremonConfig *pressConfig)
      : BrewingWebServer(pressConfig), _pressConfig(pressConfig) {}
};

// Global instance created
extern PressuremonWebServer myWebServer;

#endif  // PRESSUREMON

#endif  // SRC_WEB_PRESSUREMON_HPP_

// EOF
