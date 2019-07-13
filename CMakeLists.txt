
set(source  Source/maze.cpp)
set(binary  maze)
set(app     Maze)
set(version 0.0.0)

add_compile_options(-O3)

#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)
include(Platform/build.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------

add_executable(${binary} ${source})
target_link_libraries(${binary} ${platform_libs})
