cmake_minimum_required(VERSION 3.8)
project(vesc_speed_average)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(vesc_msgs REQUIRED)

add_executable(vesc_speed_average_node src/vesc_speed_average_node.cpp)
ament_target_dependencies(vesc_speed_average_node rclcpp vesc_msgs)

install(TARGETS
  vesc_speed_average_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  config
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
