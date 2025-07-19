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

#include "Maze.h"

//! Generate maze using tye depth first algorithm
class DepthFirstMaze : public Maze
{
public:
   using Maze::Maze;

   virtual void generate() override
   {
      std::vector<Coord> stack{};
      std::vector<Coord> neighbours;
      neighbours.reserve(4);

      std::uniform_int_distribution<> dist_w(1, getWidth() / 2);
      std::uniform_int_distribution<> dist_h(1, getHeight() / 2);

      clear(WALL);

      signed x = dist_w(rand_gen);
      signed y = dist_h(rand_gen);
      Coord pos{x, y};

      path(pos);
      stack.push_back(pos);

      while(not stack.empty())
      {
         Coord pos = stack.back();
         stack.pop_back();

         neighbours.clear();

         pos.x -= 2;
         if ((pos.x > 1) && get(pos) == WALL) neighbours.push_back(pos);
         pos.x += 4;
         if ((pos.x < (getWidth() - 2)) && get(pos) == WALL) neighbours.push_back(pos);
         pos.x -= 2;

         pos.y -= 2;
         if ((pos.y > 1) && get(pos) == WALL) neighbours.push_back(pos);
         pos.y += 4;
         if ((pos.y < (getHeight() - 2)) && get(pos) == WALL) neighbours.push_back(pos);
         pos.y -= 2;

         if (not neighbours.empty())
         {
            stack.push_back(pos);

            std::uniform_int_distribution<> dist(0, neighbours.size() - 1);

            Coord next = neighbours[dist(rand_gen)];

            path({pos.x + (next.x - pos.x) / 2, pos.y + (next.y - pos.y) / 2});
            path(next);

            stack.push_back(next);
         }
      }
   }
};
