/*
 * PressureMon
 * Copyright (c) 2021-2026 Magnus
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Alternatively, this software may be used under the terms of a
 * commercial license. See LICENSE_COMMERCIAL for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include <SH1106Wire.h>
#include <Wire.h>

#include <display.hpp>
#include <log.hpp>

Display::Display() {}

constexpr auto DISPLAY_ADRESS = 0x3C;

bool Display::isInitialized() {
  if (!_display) return false;
  return true;
}

void Display::setup() {
  if (_display) {
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

  if (_display == nullptr) {
    Log.error(F("DISP: Failed to create display driver." CR));
    return;
  }

  if (!_display->init()) {
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
