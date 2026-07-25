cmake_minimum_required(VERSION 3.8)
project(distance_computer)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(interfaces REQUIRED)

add_executable(dist_node src/distance_node.cpp)
ament_target_dependencies(dist_node rclcpp interfaces)

install(TARGETS dist_node DESTINATION lib/${PROJECT_NAME})

ament_package()
