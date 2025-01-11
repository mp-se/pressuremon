/*
MIT License

Copyright (c) 2025 Magnus

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
#ifndef SRC_I2C_MUX_HPP_
#define SRC_I2C_MUX_HPP_

#include <Arduino.h>
#include <Wire.h>

// i2c Multiplexer driver, build to support TCA9548A (8 channel mux)
//
// Addresses are selected using pins A0-A2
//
// A0   A1	 A2	  Addr
// -------------------
// LOW	LOW	 LOW	0x70
// HIGH	LOW	 LOW	0x71
// LOW	HIGH LOW	0x72
// HIGH	HIGH LOW	0x73
// LOW	LOW	 HIGH	0x74
// HIGH	LOW	 HIGH	0x75
// LOW	HIGH HIGH	0x76
// HIGH	HIGH HIGH	0x77

class I2CMux {
 private:
  TwoWire *_wire = nullptr;
  int _bus;
  int _addr;
  bool _found = false;

 public:
  explicit I2CMux(int addr = 0x70);

  bool begin(TwoWire *wire);

  bool selectBus(int bus);
  int getSelectedBus() { return _bus; }

  bool isMuxFound() { return _found; }
};

#endif  // SRC_I2C_MUX_HPP_

// EOF
