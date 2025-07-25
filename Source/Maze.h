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
      : param1(param1_)
      , param2(param2_)
      , width((width_ / 2) * 2 + 1)    // Ensure width and height are odd
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
   using Cell = uint32_t;

   static constexpr Cell PATH = 0xFFFFffff;
   static constexpr Cell WALL = 0;

   struct Coord
   {
      signed x;
      signed y;
   };

   unsigned getWidth() const { return width; }

   unsigned getHeight() const { return height; }

   void clear(Cell cell = PATH)
   {
      switch(cell)
      {
      case PATH: renderer->clear(COL_PATH); break;
      case WALL: renderer->clear(COL_WALL); break;
      }

      std::fill(map.begin(), map.end(), cell);
   }

   Cell get(unsigned x, unsigned y) const { return map[index(x, y)]; }

   Cell get(const Coord& pos) const { return map[index(pos.x, pos.y)]; }

   //! Set a cell as wall
   void wall(unsigned x, unsigned y)
   {
      map[index(x, y)] = WALL;

      if(renderer->plot(COL_WALL, x, y))
      {
         throw true;
      }
   }

   //! Set a cell as path
   void path(const Coord& pos)
   {
      map[index(pos.x, pos.y)] = PATH;

      if(renderer->plot(COL_PATH, pos.x, pos.y))
      {
         throw true;
      }
   }

   //! Set a cell as visited
   void visit(unsigned x, unsigned y, size_t dist)
   {
      map[index(x, y)] = dist;

      if(renderer->plotPal(dist, x, y))
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

      // Find starting point near the middle
      Place pos{int(width / 2), int(height / 2), 0};

      for(; pos.x < width; pos.x++)
      {
         if(get(pos.x, pos.y) == PATH)
         {
            break;
         }
      }

      size_t             max_dist{0};
      std::vector<Place> list_a{};
      std::vector<Place> list_b{};

      visit(pos.x, pos.y, pos.dist);
      list_a.push_back(pos);

      while(true)
      {
         list_b.clear();

         for(const auto& p : list_a)
         {
            if (p.dist > max_dist)
               max_dist = p.dist;

            if (get(p.x + 1, p.y) > (p.dist + 1))
            {
               visit(p.x + 1, p.y, p.dist + 1);
               list_b.push_back({p.x + 1, p.y, p.dist + 1});
            }

            if (get(p.x - 1, p.y) > (p.dist + 1))
            {
               visit(p.x - 1, p.y, p.dist + 1);
               list_b.push_back({p.x - 1, p.y, p.dist + 1});
            }

            if (get(p.x, p.y + 1) > (p.dist + 1))
            {
               visit(p.x, p.y + 1, p.dist + 1);
               list_b.push_back({p.x, p.y + 1, p.dist + 1});
            }

            if (get(p.x, p.y - 1) > (p.dist + 1))
            {
               visit(p.x, p.y - 1, p.dist + 1);
               list_b.push_back({p.x, p.y - 1, p.dist + 1});
            }
         }

         renderer->refresh();

         if (list_b.empty()) break;

         std::swap(list_a, list_b);
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

   size_t index(unsigned x, unsigned y) const { return y * width + x; }

   virtual void generate() = 0;

   static const STB::Colour COL_WALL = STB::BLACK;
   static const STB::Colour COL_PATH = STB::WHITE;

   const unsigned    width;
   const unsigned    height;
   std::vector<Cell> map;
   Renderer*         renderer{nullptr};
};
