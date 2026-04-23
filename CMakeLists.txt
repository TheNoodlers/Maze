#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(Maze
        LANGUAGES CXX
        VERSION   0.2)

if(NOT DEFINED BUILD_TESTING)
    set(BUILD_TESTING OFF CACHE BOOL "Build unit tests")
endif()
include(CTest)

add_subdirectory(PDK)
add_subdirectory(Source)
