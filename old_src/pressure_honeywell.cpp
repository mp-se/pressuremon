/*
MIT License

Copyright (c) 2021-2025 Magnus

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
/*
#include <pressure_honeywell.hpp>
#include <config.hpp>

#include <memory>

void HonewywellPressureSensor::setup(int min, int max, uint8_t idx) {
  _zeroCorrection = myConfig.getHoneywellZeroCorrection(idx);
  _idx = idx;
  _honeywellSensor.reset(new TruStabilityPressureSensor(SS, min, max));
  _honeywellSensor->begin();
  SPI.begin();
  Log.notice(
      F("PRES: Honeywell sensor, reported code %d, zero correction = %F (%d)"
CR), _honeywellSensor->status(), _zeroCorrection, _idx);
}

void HonewywellPressureSensor::loop() {
  if(!_honeywellSensor)
    return;

  switch (_honeywellSensor->readSensor()) {
    case 0:
#if LOG_LEVEL == 6
      Log.verbose(F("PRES: Sensor status NORMAL (%d)." CR), _idx);
#endif
      _sensorActive = true;
      break;
    case 1:
      Log.warning(F("PRES: Sensor status COMMAND MODE (%d)." CR), _idx);
      _sensorActive = false;
      break;
    case 2:
      Log.warning(F("PRES: Sensor status STALE DATA (%d)." CR), _idx);
      _sensorActive = false;
      break;
    case 3:
      Log.warning(F("PRES: Sensor status DIAG MODE (%d)." CR), _idx);
      _sensorActive = false;
      break;
  }
}

void HonewywellPressureSensor::calibrateSensor() {
  Log.notice(F("PRES: Starting auto calibration (%d)." CR), _idx);
  float zero = 0;

  for (int i = 0; i < 10; i++) {
    loop();
    float f = getPressurePsi(false);
    Log.notice(F("PRES: Step %d, Pressure = %F (%d)." CR), i + 1, f, _idx);
    zero += f;
    delay(500);
  }

  Log.notice(F("PRES: Measured difference %F (%d)." CR), zero / 10, _idx);
  myConfig.setHoneywellZeroCorrection(zero / 10, _idx);
  myConfig.saveFile();
  _zeroCorrection = myConfig.getHoneywellZeroCorrection(_idx);
}

float HonewywellPressureSensor::getTemperatureC() {
  if (!isSensorActive()) return 0;

  float f = _honeywellSensor->temperature();
#if LOG_LEVEL == 6
  Log.verbose(F("PRES: Reciving temp value for sensor %F C (%d)." CR), f, _idx);
#endif
  return f;
}

float HonewywellPressureSensor::getPressurePsi(bool doCorrection) {
  if (!isSensorActive()) return 0;

  float pressure = _honeywellSensor->pressure();

  if (doCorrection) return pressure - _zeroCorrection;
  return pressure;
}
*/
// EOF
