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
#ifndef _PRESSURESENSOR_H
#define _PRESSURESENSOR_H

// Includes
#include <HoneywellTruStabilitySPI.h>

// Definnitions
#define ABP_SENSOR_MIN_PRESSURE 0       // The sensor used is range 0-150 psi for the prototype, 
#define ABP_SENSOR_MAX_PRESSURE 150     // will change to 0-60 for the next build.

// classes
class PressureSensor {
    private:
        TruStabilityPressureSensor *sensor = 0;
        bool sensorActive = false;
        float zeroCorrection = 0;

    public:
        void  setup();    
        void  loop();    
        bool  isSensorActive() { return sensorActive; };

        float getPressurePsi( bool doCorrection = true );
        float getTemperatureC();

        void  calibrateSensor();

        float convertPressure2Bar( float psi ) { return psi * 0.0689475729; };
        float convertPressure2HPa( float psi ) { return psi * 68.947572932; }; 
        float convertTemperature2F(float c) { return (c * 1.8 ) + 32.0; };
};

// Global instance created
extern PressureSensor myPressureSensor;

#endif // _PRESSURESENSOR_H

// EOF