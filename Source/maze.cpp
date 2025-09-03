//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
