/*
MIT License

Copyright (c) 2021-2024 Magnus

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include <config.hpp>
#include <pressure.hpp>
#include <pressure_analog.hpp>
#include <pressure_xidibei.hpp>
#include <utils.hpp>

float convertPsiPressureToBar(float psi) { return psi * 0.0689475729; }
float convertPsiPressureToHPa(float psi) { return psi * 68.947572932; }
float convertPaPressureToPsi(float pa) { return pa * 0.000145038; }

void PressureSensor::setup(uint8_t idx, TwoWire *wire) {
  Log.verbose(F("PRES: Setting up pressuresensor index %d." CR), idx);
  bool ret = false;

  _idx = idx;

  switch (myConfig.getPressureSensorType(idx)) {
    case PressureSensorType::SensorNone:
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_300:
      _impl.reset(new XIDIBEIPressureSensor());
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(300, wire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_400:
      _impl.reset(new XIDIBEIPressureSensor());
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(400, wire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_500:
      _impl.reset(new XIDIBEIPressureSensor());
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(500, wire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_I2C_KPa_600:
      _impl.reset(new XIDIBEIPressureSensor());
      ret = static_cast<XIDIBEIPressureSensor *>(_impl.get())
                ->setup(600, wire, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_300:
      _impl.reset(new AnalogPressureSensor());
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 300, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_400:
      _impl.reset(new AnalogPressureSensor());
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 400, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_500:
      _impl.reset(new AnalogPressureSensor());
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 500, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    case PressureSensorType::SensorXidibeiXDB401_Analog_KPa_600:
      _impl.reset(new AnalogPressureSensor());
      ret = static_cast<AnalogPressureSensor *>(_impl.get())
                ->setup(0.2, 2.4, 0, 600, idx, wire,
                        idx);  // Note! Index also defines the ADC port to use
      break;

    default:
      Log.notice(F("PRES: Unknown pressure sensor id %d" CR),
                 myConfig.getPressureSensorType(idx));
      break;
  }

  if (!ret) {
    Log.error(F("PRES: Failed to communicate with sensor id %d" CR), idx);
  }
}

float PressureSensor::getPressure(bool doCorrection) {
  float p = getPressurePsi(doCorrection);

  if (myConfig.getPressureUnitAsString() == PRESSURE_BAR) {
    return convertPsiPressureToBar(p);
  } else if (myConfig.getPressureUnitAsString() == PRESSURE_HPA) {
    return convertPsiPressureToHPa(p);
  }

  return p;
}

float PressureSensor::getTemperature() {
  if (myConfig.getTempFormat() == 'C') return getTemperatureC();

  return getTemperatureF();
}

bool PressureSensor::readSensor() {
  if (_impl) {
    return _impl->readSensor();
  }

  Log.error(F("PRES: Sensor not created (%d)" CR), _idx);
  return false;
}
bool PressureSensor::isSensorActive() {
  return _impl == nullptr ? false : _impl->isSensorActive();
}

float PressureSensor::getPressurePsi(bool doCorrection) {
  return _impl == nullptr ? NAN : _impl->getPressurePsi(doCorrection);
}

float PressureSensor::getTemperatureC() {
  return _impl == 0 ? NAN : _impl->getTemperatureC();
}

float PressureSensor::getAnalogVoltage() {
  return _impl == 0 ? NAN : _impl->getAnalogVoltage();
}

void PressureSensor::calibrateSensor() {
  if (_impl) {
    _impl->calibrateSensor();
  }
}

// EOF
