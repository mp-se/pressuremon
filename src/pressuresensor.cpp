/*
MIT License

Copyright (c) 2021 Magnus

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
#include "pressuresensor.h"
#include "config.h"
#include "helper.h"

#define ABP_SENSOR_CS           D8      // 

PressureSensor myPressureSensor;

//
// Setup temp sensors
//
void PressureSensor::setup() {
#if LOG_LEVEL==6
  Log.verbose(F("PRES: Looking for pressure sensors." CR));
#endif

  zeroCorrection = myConfig.getPressureZeroCorrection();

#if !defined( SIMULATE_SENSOR )
  sensor = new TruStabilityPressureSensor( ABP_SENSOR_CS, ABP_SENSOR_MIN_PRESSURE, ABP_SENSOR_MAX_PRESSURE );
  sensor->begin();
  SPI.begin();
  Log.notice(F("PRES: Sensor reported code %d, zero correction = %f" CR), sensor->status(), zeroCorrection );
#endif
}

//
// Done in loop, reading sensor values
//
void PressureSensor::loop() {

#if defined( SIMULATE_SENSOR )
  sensorActive = true;
#else
  switch( sensor->readSensor() ) {
      case 0:
#if LOG_LEVEL==6
//       Log.verbose(F("PRES: Sensor status NORMAL." CR));
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
#endif
}

//
// Retrieving value from tempsensor within pressuresensor
//
void PressureSensor::calibrateSensor() {
  Log.notice(F("PRES: Starting auto calibration." CR));
  float zero = 0;

  for( int i = 0; i < 10; i++ ) {
    loop();
    float f = getPressurePsi( false );
    Log.notice(F("PRES: Step %d, Pressure = %F." CR), i+1, f);
    zero += f;
    delay(500);
  }

  Log.notice(F("PRES: Measured difference %F." CR), zero/10);
  char buf[20];

  dtostrf(zero/10, 5, 4, &buf[0]);
  myConfig.setPressureZeroCorrection( &buf[0] );
  myConfig.saveFile();
  zeroCorrection = myConfig.getPressureZeroCorrection();
}

//
// Retrieving value from tempsensor within pressuresensor
//
float PressureSensor::getTemperatureC() {
  if( !isSensorActive() )
    return 0;

#if defined( SIMULATE_SENSOR )
  return 20.5;
#else
  float f = sensor->temperature();
#if LOG_LEVEL==6
//  Log.verbose(F("PRES: Reciving temp value for sensor %F C." CR), f);
#endif
  return f;
#endif // SIMUALTE_SENSOR
}

//
// Retrieving value from pressure sensor
//
float PressureSensor::getPressurePsi( bool doCorrection ) {
  if( !isSensorActive() )
    return 0;

#if defined( SIMULATE_SENSOR )
  float f = 11.1;
#else
  float f = sensor->pressure();

#if LOG_LEVEL==6
//  Log.verbose(F("PRES: Reciving pressure value for sensor %F psi, correction = %F." CR), f, zeroCorrection);
#endif
#endif // SIMUALTE_SENSOR

  if( doCorrection )
    return f - zeroCorrection;

  return f;
}

// EOF 