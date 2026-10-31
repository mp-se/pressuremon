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
#ifndef SRC_PRESSURE_HPP_
#define SRC_PRESSURE_HPP_

#if defined(PRESSUREMON)

#include <Arduino.h>
#include <SoftWire.h>
#include <Wire.h>

#include <log.hpp>
#include <memory>

enum PressureSensorType {
  SensorNone = 0,

  SensorXidibeiXDB401_I2C_KPa_200 = 1,    // 0-0.2 MPa
  SensorXidibeiXDB401_I2C_KPa_400 = 2,    // 0-0.4 MPa
  SensorXidibeiXDB401_I2C_KPa_500 = 3,    // 0-0.5 MPa
  SensorXidibeiXDB401_I2C_KPa_600 = 4,    // 0-0.6 MPa
  SensorXidibeiXDB401_I2C_KPa_800 = 5,    // 0-0.8 MPa
  SensorXidibeiXDB401_I2C_KPa_1000 = 6,   // 0-1 MPa
  SensorXidibeiXDB401_I2C_KPa_1200 = 7,   // 0-1.2 MPa
  SensorXidibeiXDB401_I2C_KPa_1500 = 8,   // 0-1.5 MPa
  SensorXidibeiXDB401_I2C_KPa_1600 = 9,   // 0-1.6 MPa
  SensorXidibeiXDB401_I2C_KPa_2000 = 10,  // 0-2 MPa
  SensorXidibeiXDB401_I2C_KPa_2500 = 11,  // 0-2.5 MPa
  SensorXidibeiXDB401_I2C_KPa_3000 = 12,  // 0-3 MPa
  SensorXidibeiXDB401_I2C_KPa_3500 = 13,  // 0-3.5 MPa
  SensorXidibeiXDB401_I2C_KPa_4000 = 14,  // 0-4 MPa

  /*
  SensorXidibeiXDB401_Analog_KPa_200 = 101,   // 0-0.2 MPa
  SensorXidibeiXDB401_Analog_KPa_400 = 102,   // 0-0.4 MPa
  SensorXidibeiXDB401_Analog_KPa_500 = 103,   // 0-0.5 MPa
  SensorXidibeiXDB401_Analog_KPa_600 = 104,   // 0-0.6 MPa
  SensorXidibeiXDB401_Analog_KPa_800 = 105,   // 0-0.8 MPa
  SensorXidibeiXDB401_Analog_KPa_1000 = 106,  // 0-1 MPa
  SensorXidibeiXDB401_Analog_KPa_1200 = 107,  // 0-1.2 MPa
  SensorXidibeiXDB401_Analog_KPa_1500 = 108,  // 0-1.5 MPa
  SensorXidibeiXDB401_Analog_KPa_1600 = 109,  // 0-1.6 MPa
  SensorXidibeiXDB401_Analog_KPa_2000 = 110,  // 0-2 MPa
  SensorXidibeiXDB401_Analog_KPa_2500 = 111,  // 0-2.5 MPa
  SensorXidibeiXDB401_Analog_KPa_3000 = 112,  // 0-3 MPa
  SensorXidibeiXDB401_Analog_KPa_3500 = 113,  // 0-3.5 MPa
  SensorXidibeiXDB401_Analog_KPa_4000 = 114,  // 0-4 MPa
  */

  SensorCustom_Analog = 1000,  // Custom values
};

// Inteface towards config class for pressure sensor related settings
class PressureConfigInterface {
 public:
  virtual bool isPressureBar() const = 0;
  virtual bool isPressureKpa() const = 0;
  virtual bool isPressurePsi() const = 0;

  virtual const char* getPressureUnit() const = 0;
  virtual char getTempUnit() const = 0;

  virtual float getPressureSensorCorrection(int idx) const = 0;
  virtual void setPressureSensorCorrection(float v, int idx) = 0;

  virtual float getTemperatureSensorCorrection(int idx) const = 0;

  virtual bool saveFile() = 0;

  virtual PressureSensorType getPressureSensorType(int idx) const = 0;

  virtual float getCustomAnalogMinV(int idx) const = 0;
  virtual float getCustomAnalogMaxV(int idx) const = 0;
  virtual float getCustomAnalogMinKpa(int idx) const = 0;
  virtual float getCustomAnalogMaxKpa(int idx) const = 0;
};

class PressureSensorInterface {
 protected:
  PressureConfigInterface* _pressureConfig;

 public:
  explicit PressureSensorInterface(PressureConfigInterface* pressureConfig) {
    _pressureConfig = pressureConfig;
  }

  virtual bool read(bool validate = true) = 0;
  virtual bool isActive();
  virtual float getPressurePsi(bool doCorrection = true);
  virtual float getTemperatureC();
  virtual void calibrate();
  virtual float getAnalogVoltage();
};

class PressureSensor {
 private:
  PressureConfigInterface* _pressureConfig;
  std::unique_ptr<PressureSensorInterface> _impl;

  int _idx = 0;

 public:
  explicit PressureSensor(PressureConfigInterface* pressureConfig) {
    _pressureConfig = pressureConfig;
  }

  void setup(uint8_t idx, TwoWire* wire, SoftWire* softWire = nullptr);

  bool read();
  bool isActive();

  float getPressurePsi(bool doCorrection = true);
  float getTemperatureC();
  float getTemperatureF() { return (getTemperatureC() * 1.8) + 32.0; }

  void calibrate();

  float getPressure(
      bool doCorrection = true);  // Returns in chosen device format
  float getTemperature();         // Returns in chosen device format

  float getAnalogVoltage();
};

extern PressureSensor myPressureSensor;
#if defined(ENABLE_SECOND_SENSOR)
extern PressureSensor myPressureSensor1;
#endif

#endif  // PRESSUREMON

#endif  // SRC_PRESSURE_HPP_

// EOF
