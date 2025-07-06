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

#pragma once

#include <cstdlib>
#include <random>
#include <vector>

#include "Renderer.h"

class Maze
{
public:
   Maze(unsigned width_,
        unsigned height_,
        double   param1_,
        double   param2_)
      // Ensure width and height are odd
      : param1(param1_)
      , param2(param2_)
      , width((width_ / 2) * 2 + 1)
      , height((height_ / 2) * 2 + 1)
   {
      map.resize(width * height);
   }

   void plot(Renderer& renderer_)
   {
      renderer = &renderer_;

      clear();

      setBoundary();

      try
      {
         generate();
      }
      catch(...)
      {
      }
 
      renderer->refresh();

      renderer = nullptr;
   }

   void solve(Renderer& renderer_)
   {
      renderer = &renderer_;

      renderer->setRefreshLimit(1);

      try
      {
         doSolve();
      }
      catch(...)
      {
      }
 
      renderer->refresh();

      renderer = nullptr;
   }

protected:
   enum Cell : uint8_t
   {
      PATH    = 0,
      WALL    = 1,
      VISITED = 2
   };

   struct Coord
   {
      signed x;
      signed y;
   };

   unsigned getWidth() const { return width; }

   unsigned getHeight() const { return height; }

   Cell get(unsigned x, unsigned y) const { return map[index(x, y)]; }

   //! Set a cell as wall 
   void wall(unsigned x, unsigned y)
   {
      map[index(x,y)] = WALL;

      if (renderer->plot(COL_WALL, x, y))
      {
         throw true;
      }
   }

   //! Set a cell as visited
   void visit(unsigned x, unsigned y, size_t dist)
   {
      map[index(x,y)] = VISITED;

      if (renderer->plot(dist, x, y))
      {
         throw true;
      }
   }

   std::random_device rand_dev;
   std::mt19937       rand_gen{rand_dev()};
   double             param1;
   double             param2;

private:
   void doSolve()
   {
      struct Place
      {
         signed x, y;
         size_t dist;
      };

      // Find starting point
      Place pos{1, 1, 0};

      for(; pos.x < width; pos.x++)
      {
         if (get(pos.x, pos.y) == PATH)
         {
            break;
         }
      }

      size_t max_dist{0};
      std::vector<Place> list{};

      while(true)
      {
         if (pos.dist > max_dist)
            max_dist = pos.dist;

         visit(pos.x, pos.y, pos.dist);

         if (get(pos.x + 1, pos.y) == PATH) list.push_back({pos.x + 1, pos.y, pos.dist + 1});
         if (get(pos.x - 1, pos.y) == PATH) list.push_back({pos.x - 1, pos.y, pos.dist + 1});
         if (get(pos.x, pos.y + 1) == PATH) list.push_back({pos.x, pos.y + 1, pos.dist + 1});
         if (get(pos.x, pos.y - 1) == PATH) list.push_back({pos.x, pos.y - 1, pos.dist + 1});

         if (list.empty()) break;

         pos = list.back();
         list.pop_back();
      }

      printf("max_dist = %zu\n", max_dist);
   }

   void setBoundary()
   {
      for(size_t x = 0; x < width; x++)
      {
         wall(x, 0);
         wall(x, height - 1);
      }

      for(size_t y = 1; y < (height - 1); y++)
      {
         wall(0, y);
         wall(width - 1, y);
      }
   }

   void clear()
   {
      renderer->clear(COL_SPACE);

      std::fill(map.begin(), map.end(), PATH);
   }

   size_t index(unsigned x, unsigned y) const { return y * width + x; }

   virtual void generate() = 0;

   static const STB::Colour COL_WALL  = STB::BLACK;
   static const STB::Colour COL_SPACE = STB::WHITE;
   static const STB::Colour COL_VISIT = STB::RED;

   const unsigned    width;
   const unsigned    height;
   std::vector<Cell> map;
   Renderer*         renderer{nullptr};
};
