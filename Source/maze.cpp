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
#include "GUI/Frame.h"
#include "PLT/Event.h"

#include "PrimMaze.h"

#define  PROGRAM         "Maze"
#define  DESCRIPTION     "Maze generator"
#define  LINK            "https://github.com/AnotherJohnH/"
#define  COPYRIGHT_YEAR  "2019"
#define  AUTHOR          "John D. Haughton"

class MazeApp : public STB::ConsoleApp
{
private:
   STB::Option<unsigned> scale{      's', "scale",      "Scale",         8};
   STB::Option<unsigned> width{      'W', "width",      "Width",       101};
   STB::Option<unsigned> height{     'H', "height",     "Height",      101};
   STB::Option<double>   complexity{ 'c', "complexity", "Complexity", 0.75};
   STB::Option<double>   density{    'd', "density",    "Density",    0.75};

   virtual int startConsoleApp() override
   {
      PrimMaze   maze(width, height, complexity, density);
      GUI::Frame frame("Maze", maze.getWidth() * scale, maze.getHeight() * scale);

      maze.generate();

      frame.clear(STB::BLACK);

      maze.draw(frame, 0, 0, scale);

      frame.refresh();

      return PLT::Event::eventLoop();
   }

public:
   MazeApp(int argc, const char* argv[])
      : ConsoleApp(PROGRAM, DESCRIPTION, LINK, AUTHOR, PROJ_VERSION, COPYRIGHT_YEAR)
   {
      parseArgsAndStart(argc, argv);
   }
};


int main(int argc, const char* argv[])
{
   MazeApp(argc, argv);
}
