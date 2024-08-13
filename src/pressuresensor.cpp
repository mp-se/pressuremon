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
#include <pressuresensor.hpp>
#include <utils.hpp>

PressureSensor myPressureSensor;

void PressureSensor::setup() {
#if LOG_LEVEL == 6
  Log.verbose(F("PRES: Looking for pressure sensors." CR));
#endif

  zeroCorrection = myConfig.getPressureZeroCorrection();

  sensor = new TruStabilityPressureSensor(PIN_PRESSURE,
                                          myConfig.getPressureSensorMin(),
                                          myConfig.getPressureSensorMax());
  sensor->begin();
  SPI.begin(SCK, MISO, MOSI, SS);
  Log.notice(F("PRES: Sensor reported code %d, zero correction = %F" CR),
             sensor->status(), zeroCorrection);
}

void PressureSensor::loop() {
  switch (sensor->readSensor()) {
    case 0:
#if LOG_LEVEL == 6
      Log.verbose(F("PRES: Sensor status NORMAL." CR));
#endif
      sensorActive = true;
      break;
    case 1:
      Log.warning(F("PRES: Sensor status COMMAND MODE." CR));
      sensorActive = false;
      break;
    case 2:
      Log.warning(F("PRES: Sensor status STALE DATA." CR));
      sensorActive = false;
      break;
    case 3:
      Log.warning(F("PRES: Sensor status DIAG MODE." CR));
      sensorActive = false;
      break;
  }
}

void PressureSensor::calibrateSensor() {
  Log.notice(F("PRES: Starting auto calibration." CR));
  float zero = 0;

  for (int i = 0; i < 10; i++) {
    loop();
    float f = getPressurePsi(false);
    Log.notice(F("PRES: Step %d, Pressure = %F." CR), i + 1, f);
    zero += f;
    delay(500);
  }

  Log.notice(F("PRES: Measured difference %F." CR), zero / 10);
  myConfig.setPressureZeroCorrection(zero / 10);
  myConfig.saveFile();
  zeroCorrection = myConfig.getPressureZeroCorrection();
}

float PressureSensor::getTemperatureC() {
  if (!isSensorActive()) return 0;

  float f = sensor->temperature();
#if LOG_LEVEL == 6
  Log.verbose(F("PRES: Reciving temp value for sensor %F C." CR), f);
#endif
  return f;
}

float PressureSensor::getPressurePsi(bool doCorrection) {
  if (!isSensorActive()) return 0;

  float f = sensor->pressure();

#if LOG_LEVEL == 6
//  Log.verbose(F("PRES: Reciving pressure value for sensor %F psi, correction =
//  %F." CR), f, zeroCorrection);
#endif

  if (doCorrection) return f - zeroCorrection;
  return f;
}

float PressureSensor::getPressure(bool doCorrection) {
  float p = getPressurePsi(doCorrection);

  if (myConfig.getPressureFormatAsString() == PRESSURE_BAR) {
    return convertPressure2Bar(p);
  } else if(myConfig.getPressureFormatAsString() == PRESSURE_HPA) {
      return convertPressure2HPa(p);
  }

  return p;
}

float PressureSensor::getTemperature() {
  if (myConfig.getTempFormat() == 'C') return getTemperatureC();

  return getTemperatureF();
}

// EOF
