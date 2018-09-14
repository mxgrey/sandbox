cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Eigen3)
find_package(DART COMPONENTS gui-osg)

if(NOT Eigen3_FOUND OR
   NOT DART_FOUND)
 return()
endif()

message(STATUS "DART version: ${DART_VERSION}")

file(GLOB apps_source "*.cpp")
list(SORT apps_source)

message(STATUS "\n    DART Applications: ")
foreach(app_src_file ${apps_source})
    get_filename_component(app_base ${app_src_file} NAME_WE)
    message(STATUS "Adding application ${app_base}")
    add_executable(${app_base} ${app_src_file})
    target_link_libraries(${app_base} ${exec_dependencies})

    target_link_libraries(${app_base} PRIVATE ${DART_LIBRARIES} Eigen3::Eigen)
    target_include_directories(${app_base} PRIVATE ${DART_INCLUDE_DIRS})
    target_compile_features(${app_base} PRIVATE cxx_constexpr)
endforeach(app_src_file)

