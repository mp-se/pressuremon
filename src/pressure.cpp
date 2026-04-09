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
#include <main.hpp>
#include <pressure.hpp>
#include <pressure_analog.hpp>
#include <pressure_xidibei.hpp>

void PressureSensor::setup(uint8_t idx, TwoWire *wire, SoftWire *softWire) {
  Log.notice(F("PRES: Setting up pressuresensor index %d." CR), idx);
  bool ret = false;

  _idx = idx;

  switch (_pressureConfig->getPressureSensorType(idx)) {
    case PressureSensorType::SensorNone:
      Log.notice(F("PRES: Sensor is not configured, existing %d." CR), idx);
      return;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_200:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(200, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_400:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(400, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_500:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(500, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_600:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(600, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_800:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(800, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_1000:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(1000, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_1200:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(1200, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_1500:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(1500, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_1600:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(1600, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_2000:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(2000, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_2500:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(2500, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_3000:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(3000, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_3500:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(3500, wire, softWire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_4000:
      _impl.reset(new XIDIBEIPressureSensor(_pressureConfig));
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(4000, wire, softWire, idx);
      break;

      /*
    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_200:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 200, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_400:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 400, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_500:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 500, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_600:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 600, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_800:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 800, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_1000:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 1000, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_1200:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 1200, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_1500:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 1500, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_1600:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 1600, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_2000:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 2000, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_2500:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 2500, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_3000:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 3000, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_3500:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 3500, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_4000:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 4000, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;*/

    case PressureSensorType::SensorCustom_Analog:
      _impl.reset(new AnalogPressureSensor(_pressureConfig));
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(_pressureConfig->getCustomAnalogMinV(idx),
                        _pressureConfig->getCustomAnalogMaxV(idx),
                        _pressureConfig->getCustomAnalogMinKpa(idx),
                        _pressureConfig->getCustomAnalogMaxKpa(idx), idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    default:
      Log.notice(F("PRES: Unknown pressure sensor id %d" CR),
                 _pressureConfig->getPressureSensorType(idx));
      break;
  }

  if (!ret) {
    Log.error(F("PRES: Failed to communicate with sensor id %d" CR), idx);
    _impl.reset(); // Clean up failed sensor instance
  }
}

float PressureSensor::getPressure(bool doCorrection) {
  float p = getPressurePsi(doCorrection);

  if (_pressureConfig->isPressureBar()) {
    return convertPsiPressureToBar(p);
  } else if (_pressureConfig->isPressureKpa()) {
    return convertPsiPressureToKPa(p);
  }

  return p;
}

float PressureSensor::getTemperature() {
  if (_pressureConfig->getTempUnit() == 'C') return getTemperatureC();

  return getTemperatureF();
}

bool PressureSensor::read() {
  if (_impl.get() == nullptr) return false;

  if (isActive()) {
    return _impl->read();
  }

  Log.error(F("PRES: Sensor not created (%d)" CR), _idx);
  return false;
}
bool PressureSensor::isActive() {
  return _impl.get() == nullptr ? false : _impl->isActive();
}

float PressureSensor::getPressurePsi(bool doCorrection) {
  return _impl.get() == nullptr ? NAN : _impl->getPressurePsi(doCorrection);
}

float PressureSensor::getTemperatureC() {
  return _impl.get() == nullptr ? NAN : _impl->getTemperatureC();
}

float PressureSensor::getAnalogVoltage() {
  return _impl.get() == nullptr ? NAN : _impl->getAnalogVoltage();
}

void PressureSensor::calibrate() {
  if (isActive()) {
    _impl->calibrate();
  }
}

#endif  // PRESSUREMON

// EOF
