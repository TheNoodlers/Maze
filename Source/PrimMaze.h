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

//! Generate maze using Prim's algorithm
class PrimMaze : public Maze
{
public:
   using Maze::Maze;

   virtual void generate() override
   {
      std::uniform_int_distribution<> dist_w(0, getWidth() / 2 - 1);
      std::uniform_int_distribution<> dist_h(0, getHeight() / 2 - 1);

      std::vector<Coord> neighbours;
      neighbours.reserve(4);

      // Scale density (2nd parameter) according to the size
      unsigned density = param2 * (getWidth() / 2) * (getHeight() / 2);

      for(size_t i = 0; i < density; i++)
      {
         // Pick a random cell and mark it as part of the maze
         signed x = dist_w(rand_gen) * 2;
         signed y = dist_h(rand_gen) * 2;
         wall(x, y);

         // Scale complexity (1st parameter) according to the size
         unsigned complexity = 5 * param1 * (getWidth() + getHeight());

         for(size_t j = 0; j < complexity; j++)
         {
            neighbours.clear();

            if (x > 1)                 neighbours.push_back({x - 2, y});
            if (x < (getWidth() - 2))  neighbours.push_back({x + 2, y});
            if (y > 1)                 neighbours.push_back({x, y - 2});
            if (y < (getHeight() - 2)) neighbours.push_back({x, y + 2});

            if(not neighbours.empty())
            {
               std::uniform_int_distribution<> dist(0, neighbours.size() - 1);

               Coord pos = neighbours[dist(rand_gen)];

               if(get(pos.x, pos.y) == PATH)
               {
                  wall(pos.x, pos.y);
                  wall(pos.x + (x - pos.x) / 2, pos.y + (y - pos.y) / 2);
                  x = pos.x;
                  y = pos.y;
               }
            }
         }
      }
   }
};
