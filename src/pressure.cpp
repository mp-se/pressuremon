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
#include <pressconfig.hpp>
#include <pressure.hpp>
#include <utils.hpp>

PressureSensor myPressureSensor;

void PressureSensor::setup() {
  Log.verbose(F("PRES: Looking for pressure sensors." CR));

  switch (myConfig.getPressureSensorType()) {
    case PressureSensorType::SensorHoneywellGaugePsi_30:
    case PressureSensorType::SensorHoneywellGaugePsi_60:
    case PressureSensorType::SensorHoneywellGaugePsi_100:
    case PressureSensorType::SensorHoneywellGaugePsi_150:
      _impl = new HonewywellPressureSensor();
      _impl->setup();
      break;

    case PressureSensorType::SensorCFSensorXGZP6847DGaugeKPa_700:
    case PressureSensorType::SensorCFSensorXGZP6847DGaugeKPa_1000:
      _impl = new CFSensorPressureSensor();
      _impl->setup();
      break;

    default:
      Log.notice(F("PRES: Unknown pressure sensor id %d" CR),
                 myConfig.getPressureSensorType());
      break;
  }
}

float PressureSensor::getPressure(bool doCorrection) {
  float p = getPressurePsi(doCorrection);

  if (myConfig.getPressureUnitAsString() == PRESSURE_BAR) {
    return convertPressure2Bar(p);
  } else if (myConfig.getPressureUnitAsString() == PRESSURE_HPA) {
    return convertPressure2HPa(p);
  }

  return p;
}

float PressureSensor::getTemperature() {
  if (myConfig.getTempFormat() == 'C') return getTemperatureC();

  return getTemperatureF();
}

// EOF
