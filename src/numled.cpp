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
#include "numled.h"

NumLED myNumLED;

//
// Constructor. This is build to handle a TM1637 compatible display.
//
void NumLED::setup() {
//#if LOG_LEVEL==6
//  Log.verbose(F("DISP: Initilizing LED." CR));
//#endif
  led = new TM1637Display( D1, D2 ); // CLK, DIO (Standad pins on ESP8266)
  Log.notice(F("DISP: Setting up LED display." CR));
  led->setBrightness(0x0f);
  led_found = true;
#ifdef DISPLAY_SELFTEST 
  selfTest();
#endif
  clear();
}

//
// Testing functionality of the displayclass
//
#ifdef DISPLAY_SELFTEST 
void NumLED::selfTest() {
  printNum( 9999 );
  delay(2000);
  printNum( (float) 99.99 );
  delay(2000);
  printDash( 1 );
  delay(2000);
  printDash( 2 );
  delay(2000);
  printDash( 3 );
  delay(2000);
  printDash( 4 );
  delay(2000);
  printConn();
  delay(2000);
  printErr();
  delay(2000);
  printUpd();
  delay(2000);
}
#endif 

//
// clear the display
//
void NumLED::clear() {
#if LOG_LEVEL==6
//  Log.verbose(F("DISP: Clear display." CR));
#endif

  if( !led_found )
    return;

  led->clear();
}

//
// Print value
//
void NumLED::printNum( int v ) {
#if LOG_LEVEL==6
//  Log.verbose(F("DISP: Printing '%d." CR), v);
#endif

  if( !led_found )
    return;

//led->showNumberDec( v );   
  led->showNumberDecEx( v, 0b00000000, true);
}

//
// Print value
//
void NumLED::printNum( float f ) {
#if LOG_LEVEL==6
//  Log.verbose(F("DISP: Printing '%d." CR), v);
#endif

  if( !led_found )
    return;

  // We should not have a negative pressure, show this as Zero.
  if( f < 0 )
    f = 0;

  f *= 100;
  led->showNumberDecEx( (int) f, 0b01000000, true);
}

//
// Print text
//
void NumLED::printConn() {
#if LOG_LEVEL==6
//  Log.verbose(F("DISP: Printing Conn." CR));
#endif

  if( !led_found )
    return;

  // Segment codes
  //       _ a
  //    f |g| b
  //    e |_| c
  //       d
  
  /*const uint8_t SEG_DONE[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
    SEG_C | SEG_E | SEG_G,                           // n
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
    };*/

  const uint8_t SEG_CONN[] = {
    SEG_F | SEG_E | SEG_A | SEG_D,                   // C
    SEG_E | SEG_D | SEG_C | SEG_G,                   // o
    SEG_E | SEG_G | SEG_C,                           // n
    SEG_E | SEG_G | SEG_C                            // n
    };

  led->setSegments( SEG_CONN );   
}

//
// Print text
//
void NumLED::printErr() {
#if LOG_LEVEL==6
//  Log.verbose(F("DISP: Printing Err." CR));
#endif

  if( !led_found )
    return;

  // Segment codes
  //       _ a
  //    f |g| b
  //    e |_| c
  //       d
  
  const uint8_t SEG_ERR[] = {
    SEG_F | SEG_E | SEG_A | SEG_D | SEG_G,           // E
    SEG_E | SEG_G,                                   // r
    SEG_E | SEG_G,                                   // r
    0                                                //  
    };

  led->setSegments( SEG_ERR );   
}

//
// Print text
//
void NumLED::printDash( int n ) {
#if LOG_LEVEL==6
//  Log.verbose(F("DISP: Printing dash." CR));
#endif

  if( !led_found )
    return;

  // Segment codes
  //       _ a
  //    f |g| b
  //    e |_| c
  //       d
  
  uint8_t SEG_DASH[] = {
    0, 0, 0, 0  
    };

  for( int i = 0; i < n; i ++)
    SEG_DASH[i] = SEG_G;

  led->setSegments( SEG_DASH );   
}

//
// Print text
//
void NumLED::printUpd() {
#if LOG_LEVEL==6
//  Log.verbose(F("DISP: Printing upd." CR));
#endif

  if( !led_found )
    return;

  // Segment codes
  //       _ a
  //    f |g| b
  //    e |_| c
  //       d
  
  uint8_t SEG_UPD[] = {
    SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,      // U
    SEG_F | SEG_E | SEG_A | SEG_B | SEG_G,      // p
    SEG_E | SEG_D | SEG_C | SEG_B | SEG_G,      // d
    0                                           // 
    };

  led->setSegments( SEG_UPD );   
}

// EOF 