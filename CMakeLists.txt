cmake_minimum_required(VERSION 3.5)
project(3DRender)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -O3 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(JSONCPP_WITH_TESTS OFF)
add_subdirectory(externals/jsoncpp)
include_directories(externals/jsoncpp/include)

add_subdirectory(src)
