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

#include "STB/ConsoleApp.h"

#include "Renderer.h"

#include "PrimMaze.h"
#include "DepthFirstMaze.h"

static const char* PROGRAM        = "Maze";
static const char* DESCRIPTION    = "Maze generator";
static const char* LINK           = "https://github.com/TheNoodlers/Maze";
static const char* AUTHOR         = "John D. Haughton";
static const char* COPYRIGHT_YEAR = "2019";

class MazeApp : public STB::ConsoleApp
{
public:
   MazeApp()
      : ConsoleApp(PROGRAM, DESCRIPTION, LINK, AUTHOR, COPYRIGHT_YEAR)
   {
   }

private:
   STB::Option<unsigned>    width{     'W', "width",      "Width",       400};
   STB::Option<unsigned>    height{    'H', "height",     "Height",      400};
   STB::Option<unsigned>    scale{     's', "scale",      "Scale",         2};
   STB::Option<double>      complexity{'c', "complexity", "Complexity", 0.75};
   STB::Option<double>      density{   'd', "density",    "Density",    0.75};
   STB::Option<const char*> alg{       'a', "algorithm",  "Algorithm",  "P"};

   template <typename TYPE>
   void plotAndSolve(Renderer& renderer_)
   {
      TYPE maze{width, height, complexity, density};

      maze.plot(renderer_);
      maze.solve(renderer_);
   }

   virtual int startConsoleApp() override
   {
      Renderer renderer(PROGRAM, width, height, scale);

           if (strcmp(alg, "P") == 0) plotAndSolve<PrimMaze>(renderer);
      else if (strcmp(alg, "D") == 0) plotAndSolve<DepthFirstMaze>(renderer);

      return renderer.waitForQuit();
   }
};

int main(int argc, const char* argv[])
{
   return MazeApp().parseArgsAndStart(argc, argv);
}
