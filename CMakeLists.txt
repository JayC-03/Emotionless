cmake_minimum_required(VERSION 2.8.8)
project(emotionless)

include_directories(src)

option(USE_BIOS_HLE "Emulate the PS2's BIOS using HLE" ON)

if(USE_BIOS_HLE)
	add_definitions(-DUSE_BIOS_HLE)
endif()

add_definitions(-std=gnu++0x)

add_subdirectory(src)
