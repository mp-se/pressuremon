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
#ifndef _NUMLED_H
#define _NUMLED_H

// include
#include "helper.h"

#include <Wire.h>                   
#include <TM1637Display.h>

// classes
class NumLED {
    private:
        TM1637Display *led;;
        bool led_found = false;

#ifdef DISPLAY_SELFTEST 
        void selfTest();
#endif

    public:
        void setup();
        void clear();

        void printNum( int v );
        void printNum( float f );
        void printConn();               // Show the word Conn on LED (used to indicate connection to WIFI)
        void printErr();                // Show the word Err on LED (used to indicate Error)
        void printUpd();                // Show the word Upd on LED (used to indicate Error)
        void printDash( int n = 1);     // Show one or more - on LED (used to indicate progress), i.e. (-, --, ---, ----)
};

extern NumLED myNumLED;

#endif // _NUMLED_H

// EOF