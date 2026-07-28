cmake_minimum_required(VERSION 3.8)
project(servo_actuator)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(interfaces REQUIRED)

add_executable(serial src/actuator.cpp)
ament_target_dependencies(serial rclcpp interfaces)

install(TARGETS serial DESTINATION lib/${PROJECT_NAME})

ament_package()
