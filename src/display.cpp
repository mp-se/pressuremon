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
#include <display.hpp>
#include <SH1106Wire.h>
#include <Wire.h>
#include <log.hpp>

Display::Display() {}

constexpr auto DISPLAY_ADRESS = 0x3C;

bool Display::isInitialized() {
  if (!_display) return false;
  return true;
}

void Display::setup() {
  if(_display) {
    delete _display;
    _display = nullptr;
  }

  Wire.beginTransmission(DISPLAY_ADRESS);
  int err = Wire.endTransmission();
  if (err) {
    Log.warning(F("DISP: No display found on I2C bus." CR));
    return;
  }

  _display = new SH1106Wire(DISPLAY_ADRESS);
  _fontSize = FontSize::FONT_10;
  _width = 127;
  _height = 63;

  if(_display == nullptr) {
    Log.error(F("DISP: Failed to create display driver." CR));
    return;
  }

  if(!_display->init()) {
    Log.error(F("DISP: Failed to initialize display driver." CR));
    return;
  }
  _display->displayOn();
  _display->flipScreenVertically();

  clear();
  show();
}

void Display::setFont(FontSize fs) {
  if (!isInitialized()) return;

  _fontSize = fs;

  switch (fs) {
    case FontSize::FONT_1:
      return;

    case FontSize::FONT_10:
      _display->setFont(ArialMT_Plain_10);
      return;

    case FontSize::FONT_16:
      _display->setFont(ArialMT_Plain_16);
      return;

    case FontSize::FONT_24:
      _display->setFont(ArialMT_Plain_24);
      return;
  }
}

int Display::getTextWidth(const String& text) {
  int w = 0;
  if (!isInitialized()) return -1;

  w = _display->getStringWidth(text);
  return w;
}

void Display::printPosition(int x, int y, const String& text) {
  if (!isInitialized()) return;

  if (x < 0) {
    int w = getTextWidth(text);
    x = (_width - w) / 2;
  }

  _display->drawString(x, y, text);
}

void Display::printLine(int l, const String& text) {
  if (!isInitialized()) return;

  printPosition(0, _fontSize * l, text);
}

void Display::printLineCentered(int l, const String& text) {
  if (!isInitialized()) return;

  int w = getTextWidth(text);

  printPosition((_width - w) / 2, _fontSize * l, text);
}

void Display::clear() {
  if (!isInitialized()) return;

  _display->clear();
}

void Display::show() {
  if (!isInitialized()) return;

  _display->display();
}

// EOF
