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
