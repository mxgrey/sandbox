cmake_minimum_required(VERSION 3.5.1)
project(sandbox)

add_definitions("-Wall -Wextra")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

list(APPEND CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
#list(APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

file(GLOB apps_source "apps/*.cpp")
LIST(SORT apps_source)

message(STATUS "\n    Applications: ")
foreach(app_src_file ${apps_source})
    get_filename_component(app_base ${app_src_file} NAME_WE)
    message(STATUS "Adding application ${app_base}")
    add_executable(${app_base} ${app_src_file})
    target_link_libraries(${app_base} ${exec_dependencies})
endforeach(app_src_file)

target_compile_options(test_ShadowVirtualFunction PRIVATE -Woverloaded-virtual)

add_subdirectory(spl)
add_subdirectory(ign-common)
add_subdirectory(dart)
