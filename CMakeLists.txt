cmake_minimum_required(VERSION 3.5)
project(3DRENDER)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(.)

aux_source_directory(. SRCS)
aux_source_directory(common COMMON_SRCS)
aux_source_directory(light LIGHT_SRCS)
aux_source_directory(object OBJECT_SRCS)

add_executable(main ${SRCS} ${COMMON_SRCS} ${LIGHT_SRCS} ${OBJECT_SRCS})
