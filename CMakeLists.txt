cmake_minimum_required(VERSION 2.8.3)
project(sandbox)

add_definitions("-Wall -Wextra")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
#list(APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

file(GLOB apps_source "apps/*.cpp")
LIST(SORT apps_source)

add_subdirectory(spl)

#message(STATUS "\n    Applications: ")
#foreach(app_src_file ${apps_source})
#    get_filename_component(app_base ${app_src_file} NAME_WE)
#    message(STATUS "Adding application ${app_base}")
#    add_executable(${app_base} ${app_src_file})
#    target_link_libraries(${app_base} ${exec_dependencies})
#endforeach(app_src_file)

add_executable(test_TemplateMemberShadow apps/test_TemplateMemberShadow.cpp)
add_executable(test_ConditionalMemberFunction apps/test_ConditionalMemberFunction.cpp)
add_executable(test_ConditionalPacking apps/test_ConditionalPacking.cpp)
add_executable(test_NestedTemplateParameters apps/test_NestedTemplateParameters.cpp)
add_executable(test_Aspect apps/test_Aspect.cpp)
add_executable(test_ContainerTemplate apps/test_ContainerTemplate.cpp)
add_executable(test_string_set_vs_unordered apps/test_string_set_vs_unordered.cpp)
add_executable(test_VariadicTemplateToArray apps/test_VariadicTemplateToArray.cpp)
