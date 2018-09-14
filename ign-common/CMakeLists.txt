cmake_minimum_required(VERSION 3.5.1)

# This is for testing applications that use ign-common

message(STATUS "\n")

find_package(ignition-math5 COMPONENTS eigen3)
if(NOT ignition-math5_FOUND)
  return()
endif()

add_executable(dummy dummy.cpp)

target_link_libraries(dummy ignition-math5::eigen3)
