# Maze

![screenshot](Docs/Screenshot.png)

## Source checkout

This repo uses git sub-modules, so checkout using --recurse to clone all the
dependent source...

    git clone --recurse https://github.com/TheNoodlers/Maze.git

or

    git clone --recurse ssh://git@github.com/TheNoodlers/Maze.git

## Software dependencies

+ https://github.com/AnotherJohnH/Platform
+ **cmake** via UNIX **make** or auto detection of **ninja** if installed
+ Python3
+ SDL2

## Build

Developed on MacOS but should build on Linux too.

Indirect build with **cmake** and **make** or **ninja**...

    make

Direct build using cmake...

    mkdir build
    cd build
    cmake ..
    make
