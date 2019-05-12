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

#ifndef PRIM_MAZE_H
#define PRIM_MAZE_H

#include <cstdlib>

#include "Maze.h"

class PrimMaze : public Maze
{
public:
   PrimMaze(unsigned width_,
        unsigned height_,
        double   complexity_ = 0.75,
        double   density_ = 0.75)
      : Maze(width_, height_)
      // Scale complexity and density according to the size
      , complexity(5 * complexity_ * (width + height))
      , density(density_ * (width / 2) * (height / 2))
   {
   }

   virtual void generate() override
   {
      struct Coord
      {
         signed x;
         signed y;
      };

      std::vector<Coord> neighbours;
      neighbours.reserve(4);

      for(size_t i=0; i<density; i++)
      {
         signed x = rand() % (width  / 2) * 2;
         signed y = rand() % (height / 2) * 2;

         set(x, y);

         for(size_t j=0; j<complexity; j++)
         {
            neighbours.clear();

            if (x > 1)            neighbours.push_back({x - 2, y});
            if (x < (width - 2))  neighbours.push_back({x + 2, y});
            if (y > 1)            neighbours.push_back({x, y - 2});
            if (y < (height - 2)) neighbours.push_back({x, y + 2});

            if (!neighbours.empty())
            {
               Coord pos = neighbours[rand() % neighbours.size()];
               if (!get(pos.x, pos.y))
               {
                  set(pos.x, pos.y);
                  set(pos.x + (x - pos.x)/2, pos.y + (y - pos.y)/2);
                  x = pos.x;
                  y = pos.y;
               }
            }
         }
      }
   }

private:
   const unsigned complexity;
   const unsigned density;
};

#endif
