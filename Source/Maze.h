//------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
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

#ifndef MAZE_H
#define MAZE_H

#include <cstdlib>
#include <vector>

class Maze
{
public:
   Maze(unsigned width_,
        unsigned height_)
      // Ensure width and height are odd
      : width((width_ / 2) * 2 + 1)
      , height((height_ / 2) * 2 + 1)
   {
      // Prepare an empty maze
      maze.resize(width * height);

      for(size_t x=0; x<width; x++)
      {
         for(size_t y=0; y<height; y++)
         {
            set(x, y, ((x == 0) || (x == (width - 1)) || (y == 0) || (y == (height - 1))));
         }
      }
   }

   unsigned getWidth() const { return width; }

   unsigned getHeight() const { return height; }

   bool get(unsigned x, unsigned y) const { return maze[index(x,y)]; }

   virtual void generate() = 0;

   void draw(GUI::Canvas& canvas, unsigned ox, unsigned oy, unsigned scale)
   {
      for(unsigned x=0; x<getWidth(); x++)
      {
         unsigned px = ox + x * scale;

         for(unsigned y=0; y<getHeight(); y++)
         {
            unsigned py = oy + y * scale;

            canvas.fillRect(get(x, y) ? STB::BLACK : STB::WHITE,
                           px, py, px + scale, py + scale);
         }
      }
   }

protected:
   const unsigned width;
   const unsigned height;

   size_t index(unsigned x, unsigned y) const { return y * width + x; }

   void set(unsigned x, unsigned y, bool value = true)
   {
      maze[index(x,y)] = value;
   }

   void clr(unsigned x, unsigned y)
   {
      set(x, y, false);
   }

private:
   std::vector<bool> maze;
};

#endif
