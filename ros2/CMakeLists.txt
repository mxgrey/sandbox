cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

find_package(rclcpp)
find_package(std_msgs)
find_package(example_interfaces)

foreach(dep rclcpp std_msgs example_interfaces)

  if(NOT ${dep}_FOUND)
    message("Could not find ${dep} so we will skip ros2 examples")
    return()
  endif()

endforeach()

file(GLOB apps_source "*.cpp")
LIST(SORT apps_source)

message(STATUS "\n    ROS2 Applications: ")
foreach(app_src_file ${apps_source})
    get_filename_component(app_base ${app_src_file} NAME_WE)
    message(STATUS "Adding application ${app_base}")
    add_executable(${app_base} ${app_src_file})
    target_link_libraries(${app_base} ${exec_dependencies})

    target_link_libraries(${app_base}
      PRIVATE
        ${rclcpp_LIBRARIES}
        ${std_msgs_LIBRARIES}
        ${example_interfaces_LIBRARIES}
    )

    target_include_directories(${app_base}
      PRIVATE
        ${rclcpp_INCLUDE_DIRS}
        ${std_msgs_INCLUDE_DIRS}
        ${example_interfaces_INCLUDE_DIRS}
    )

    target_compile_features(${app_base} PRIVATE cxx_std_14)
endforeach(app_src_file)
