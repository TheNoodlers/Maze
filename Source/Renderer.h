//------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#pragma once

#include <ctime>

#include "GUI/Frame.h"
#include "PLT/Event.h"

class Renderer
{
public:
   Renderer(const char* title_,
            unsigned    width_,
            unsigned    height_,
            unsigned    scale_)
      : frame(title_, width_ * scale_, height_ * scale_)
      , scale(scale_)
   {
      clear(STB::BLACK);

      buildPalette();

      refresh_time = clock();
   }

   //! Clear frame to a single colour
   void clear(STB::Colour colour)
   {
      frame.clear(colour);
   }

   //! Plot a single point in the frame
   bool plot(STB::Colour colour, unsigned x, unsigned y)
   {
      unsigned px = x * scale;
      unsigned py = y * scale;

      frame.fillRect(colour, px, py, px + scale, py + scale);

      if (isRefresh())
      {
         refresh();

         // Poll for quit at ~1 Hz
         if (++n >= REFRESH_FREQ)
         {
            n = 0;
            return pollForQuit();
         }
      }

      return false;
   }

   //! Plot a single point in the frame
   bool plotPal(size_t index, unsigned x, unsigned y)
   {
      return plot(palette[index % PALETTE_SIZE], x, y);
   }

   //! Refresh the frame to the display
   void refresh()
   {
      frame.refresh();
   }

   //! Poll for a quit event
   bool pollForQuit()
   {
      PLT::Event::Message msg;

      return PLT::Event::poll(msg) == PLT::Event::QUIT;
   }

   //! Wait for a quit event
   int waitForQuit()
   {
      return PLT::Event::eventLoop();
   }

private:
   void buildPalette()
   {
      for(unsigned bgr = 0; bgr < 7; ++bgr)
      {
         uint8_t fr_red = ((bgr >> 0) & 1) * 0xFF;
         uint8_t fr_grn = ((bgr >> 1) & 1) * 0xFF;
         uint8_t fr_blu = ((bgr >> 2) & 1) * 0xFF;

         uint8_t to_red = (((bgr + 1) >> 0) & 1) * 0xFF;
         uint8_t to_grn = (((bgr + 1) >> 1) & 1) * 0xFF;
         uint8_t to_blu = (((bgr + 1) >> 2) & 1) * 0xFF;

         for(unsigned i = 0; i < 256; ++i)
         {
            uint8_t red = (fr_red * (255 - i) + to_red * i) / 255;
            uint8_t grn = (fr_grn * (255 - i) + to_grn * i) / 255;
            uint8_t blu = (fr_blu * (255 - i) + to_blu * i) / 255;

            palette[PALETTE_SIZE - (bgr * 256 + i) - 1] = STB::RGB(red, grn, blu);
         }
      }
   }

   bool isRefresh()
   {
      signed remainder = refresh_time - clock();
      if (remainder > 0)
         return false;

      scheduleRefresh();
      return true;
   }

   void scheduleRefresh()
   {
      refresh_time = clock() + (CLOCKS_PER_SEC / REFRESH_FREQ);
   }

   static const unsigned REFRESH_FREQ = 20;
   static const unsigned PALETTE_SIZE = 0x700;

   GUI::Frame  frame;
   unsigned    scale;
   clock_t     refresh_time;
   unsigned    n{REFRESH_FREQ};
   STB::Colour palette[PALETTE_SIZE];
};
