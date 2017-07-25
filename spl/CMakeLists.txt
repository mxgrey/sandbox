cmake_minimum_required(VERSION 2.8.3)

# SPL == System Plugin Library

get_filename_component(dir_name ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB lib_source "*.cpp" "*.h*")
LIST(SORT lib_source)

message(STATUS "Configuring library: ${dir_name}")
add_library(${dir_name} ${lib_source})

set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(HEADER_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${dir_name} DESTINATION ${LIBRARY_INSTALL_PATH})
