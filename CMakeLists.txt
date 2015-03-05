cmake_minimum_required(VERSION 2.8.8)
project(emotionless)

include_directories(src)

add_definitions(-std=gnu++0x -DUSE_HLE)

add_subdirectory(src)