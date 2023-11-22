cmake_minimum_required(VERSION 3.26)
project(NTPClient)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(NTPClient
        NTPClient.cpp
        NTPClient.h)
