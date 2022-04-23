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

#include <string>

#include "GUI/Frame.h"
#include "PLT/Event.h"

class Renderer
{
public:
   Renderer(const std::string& title_,
            unsigned           width_,
            unsigned           height_,
            unsigned           scale_)
      : frame(title_.c_str(), width_ * scale_, height_ * scale_)
      , scale(scale_)
   {
      clear(STB::BLACK);
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

      if (++n > REFRESH_LIMIT)
      {
         n = 0;
         refresh();

         return pollForQuit();
      }

      return false;
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
   static const unsigned REFRESH_LIMIT = 500;

   GUI::Frame frame;
   unsigned   scale;
   unsigned   n{};
};
