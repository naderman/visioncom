
cmake_minimum_required(VERSION 2.6)

set(VISIONCOM_CMAKE_DIRECTORY "../cmake")
include(${VISIONCOM_CMAKE_DIRECTORY}/build.cmake)

add_subdirectory(../src visioncom)
