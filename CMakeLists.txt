cmake_minimum_required(VERSION 2.8.3)
project(sandbox)

add_definitions("-Wall -Wextra")

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
#list(APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

file(GLOB apps_source "apps/*.cpp")
LIST(SORT apps_source)

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
