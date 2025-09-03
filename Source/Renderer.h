//------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
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
      , refresh_time(clock())
   {
      clear(STB::BLACK);

      buildPalette();
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

      if(isRefreshTime())
      {
         refresh();

         return pollForQuit();
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

   bool isRefreshTime()
   {
      signed remainder = refresh_time - clock();
      if (remainder > 0)
         return false;

      refresh_time = clock() + (CLOCKS_PER_SEC / REFRESH_FREQ);
      return true;
   }

   static const unsigned REFRESH_FREQ = 20;
   static const unsigned PALETTE_SIZE = 0x700;

   GUI::Frame  frame;
   unsigned    scale;
   clock_t     refresh_time;
   STB::Colour palette[PALETTE_SIZE];
};
