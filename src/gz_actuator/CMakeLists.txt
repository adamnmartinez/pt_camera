cmake_minimum_required(VERSION 3.8)
project(gz_actuator)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(interfaces REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(actuator src/sim_actuator.cpp)
ament_target_dependencies(actuator rclcpp interfaces std_msgs)

install(TARGETS actuator DESTINATION lib/${PROJECT_NAME})

ament_package()
