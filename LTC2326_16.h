/**************************************************************************/
/*

LTC2326-16 library for Teensy 3.1
Last updated Oct 14, 2015
http://dberard.com/home-built-stm/


 * Copyright (c) Daniel Berard, daniel.berard@mail.mcgill.ca
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.

*/
/**************************************************************************/

#ifndef LTC2326_16_h
#define LTC2326_16_h

#include "Arduino.h"
#include <SPIFIFO.h>

class LTC2326_16
{
  
  public:
    
    LTC2326_16(byte cs, byte cnv, byte busy); // Constructor
    void convert(); // Initiate conversion
    bool busy(); // Check the status of the current conversion
    void begin(); // Setup the SPI port
    int16_t read(); // Read the ADC data register
    
  private:
    
    byte _cs;
    byte _cnv;
    byte _busy;
  
};

#endif
