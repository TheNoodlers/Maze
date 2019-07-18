
set(source  Source/maze.cpp)
set(binary  maze)
set(app     Maze)
set(version 0.0.0)

#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.11.4)
include(Platform/build.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------

add_executable(${binary} ${source})
target_link_libraries(${binary} ${platform_libs})
