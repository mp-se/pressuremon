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
#include <pressure_cfsensor.hpp>
#include <pressure_xidibei.hpp>
#include <utils.hpp>

// TODO: Only digital or even analoge ? What is the difference i resolution ?

float convertPsiPressureToBar(float psi) { return psi * 0.0689475729; }
float convertPsiPressureToHPa(float psi) { return psi * 68.947572932; }
float convertPaPressureToPsi(float pa) { return pa * 0.000145038; }

void PressureSensor::setup(uint8_t idx, I2CMux *mux) {
  Log.verbose(F("PRES: Setting up pressuresensor index %d." CR), idx);
  _idx = idx;
  _mux = mux;

  switch (myConfig.getPressureSensorType(idx)) {
    case PressureSensorType::SensorCFSensorXGZP6847DGaugeKPa_700:
    case PressureSensorType::SensorCFSensorXGZP6847DGaugeKPa_1000:
      _impl.reset(new CFSensorPressureSensor());
      static_cast<CFSensorPressureSensor *>(_impl.get())->setup(64, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_KPa_300:
      _impl.reset(new XIDIBEIPressureSensor());
      static_cast<XIDIBEIPressureSensor *>(_impl.get())->setup(300, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_KPa_400:
      _impl.reset(new XIDIBEIPressureSensor());
      static_cast<XIDIBEIPressureSensor *>(_impl.get())->setup(400, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_KPa_500:
      _impl.reset(new XIDIBEIPressureSensor());
      static_cast<XIDIBEIPressureSensor *>(_impl.get())->setup(500, idx);
      break;

    case PressureSensorType::SensorXidibeiXDB401_KPa_600:
      _impl.reset(new XIDIBEIPressureSensor());
      static_cast<XIDIBEIPressureSensor *>(_impl.get())->setup(600, idx);
      break;

    case PressureSensorType::SensorNone:
      break;

    default:
      Log.notice(F("PRES: Unknown pressure sensor id %d" CR),
                 myConfig.getPressureSensorType(idx));
      break;
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
    if(_mux) {
      _mux->selectBus(_idx);
    } else {
      Log.error(F("PRES: Multiplexer not defined (%d)" CR), _idx);
    }

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

void PressureSensor::calibrateSensor() {
  if (_impl) {
    if(_mux) _mux->selectBus(_idx);
    _impl->calibrateSensor();
  }
}

// EOF
