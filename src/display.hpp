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
#ifndef SRC_DISPLAY_HPP_
#define SRC_DISPLAY_HPP_

#include <Arduino.h>

class SH1106Wire;

enum FontSize {  // Font options
  FONT_1 = 1,    // Support LCD
  FONT_10 = 10,  // Support OLED 6 lines
  FONT_16 = 16,  // Support OLED 5 lines
  FONT_24 = 24   // Support OLED 3 lines
};

class Display {
 private:
  SH1106Wire* _display = nullptr;
  FontSize _fontSize = FontSize::FONT_10;
  int _width = 0;
  int _height = 0;

 public:
  Display();
  void setup();
  void clear();
  void show();

  bool isInitialized();

  void setFont(FontSize fs);
  int getFontHeight() { return _fontSize; }
  int getTextWidth(const String& text);

  int getDisplayWidth() { return _width; }
  int getDisplayHeight() { return _height; }

  void printPosition(int x, int y, const String& text);
  void printLine(int l, const String& text);
  void printLineCentered(int l, const String& text);
};

extern Display myDisplay;

#endif  // SRC_DISPLAY_HPP_

// EOF
