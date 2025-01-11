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
#include <i2c_mux.hpp>
#include <log.hpp>

I2CMux::I2CMux(int addr) { _addr = addr; }

bool I2CMux::begin(TwoWire *wire) {
  int retry = 0;

  _wire = wire;
  _bus = 0;

  do {
    if(retry > 3) {
      Log.notice(F("MUX : Multiplexer NOT found after 3 retries." CR));
      return false;
    }

    _wire->beginTransmission(_addr);
    _wire->write(1 << 0);
    _found = _wire->endTransmission() == 0;
    retry++;
  } while(!_found);

  Log.notice(F("MUX : Multiplexer found." CR));
  return true;
}

bool I2CMux::selectBus(int bus) {
  if (_wire && _found) {
    Log.notice(F("MUX : Selecting i2c bus %d." CR), bus);

    _bus = bus;
    _wire->beginTransmission(_addr);
    _wire->write(1 << _bus);
    return _wire->endTransmission() == 0;
  } else {
    Log.notice(F("MUX : Unable to select bus %d." CR), bus);
  }

  return false;
}

// EOF
