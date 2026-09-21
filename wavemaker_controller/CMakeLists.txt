cmake_minimum_required(VERSION 3.8)
project(wavemaker_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(wavemaker_interfaces REQUIRED)
find_package(bondcpp REQUIRED)   

add_executable(wavemaker_node src/wavemaker_node.cpp)
ament_target_dependencies(wavemaker_node
  lifecycle_msgs
  rclcpp
  rclcpp_lifecycle
  rclcpp_action
  wavemaker_interfaces
  bondcpp
)

install(TARGETS wavemaker_node
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()