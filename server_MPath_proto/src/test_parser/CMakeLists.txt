#cmake_minimum_required(VERSION 2.8)

project (test_parser)

add_compile_options(-std=c++11)

#for debugging
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV {CXXFLAGS} -O3 -wall")

add_executable(test_parser codeStreaming_parser.cpp)