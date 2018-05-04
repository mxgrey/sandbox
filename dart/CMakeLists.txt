cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

project(sandbox-dart)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(EIGEN3)
find_package(DART)

message(STATUS "DART version: ${DART_VERSION}")

add_executable(simple simple.cpp)

target_link_libraries(simple PRIVATE ${DART_LIBRARIES})
target_include_directories(simple PRIVATE ${EIGEN3_INCLUDE_DIRS})
target_compile_features(simple PRIVATE cxx_constexpr)
