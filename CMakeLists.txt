cmake_minimum_required(VERSION 2.8.8)
project(emotionless)

include_directories(src)

option(USE_BIOS_HLE "Emulate the PS2's BIOS using HLE" ON)
option(USE_JIT "Emulate the PS2's CPUs using JITs" OFF)
option(PROFILING "Enable profiling" OFF)

add_definitions(-fpermissive)

if(CMAKE_BUILD_TYPE STREQUAL Release)
	add_definitions(-O3)
endif()

if(USE_BIOS_HLE)
	add_definitions(-DUSE_BIOS_HLE)
endif()

if(USE_JIT)
	add_definitions(-DUSE_JIT)
endif()

if(PROFILING)
	add_definitions(-pg)
endif()

add_definitions(-std=gnu++0x)

add_subdirectory(src)
