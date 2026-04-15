
cmake_minimum_required(VERSION 3.20)

project(Maze
        LANGUAGES CXX
        VERSION   0.2)

add_subdirectory(PDK)

add_executable(maze Source/maze.cpp)

target_link_libraries(maze GUI)
