cmake_minimum_required(VERSION 3.16)
project(DG)

# enable c++20 features
set(CMAKE_CXX_STANDARD 20)

# compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic")

# global include directories
include_directories(${PROJECT_SOURCE_DIR}/src)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# compile files in src as a library
# file(GLOB_RECURSE DG_SOURCE *.cpp)
# add_library(DG ${DG_SOURCE})
add_library(DG INTERFACE) # for headers only library

# examples for more cmake info
add_subdirectory(examples)
# add_subdirectory(src)

# test
option(DG_TESTS "tests" ON)
if(DG_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

