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
find_package(mgate5101_driver REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)

add_executable(wavemaker_node
  src/wavemaker_node.cpp
  src/indradrive_actuator.cpp
  src/wave_math.cpp
  src/wave_trajectory.cpp
)
ament_target_dependencies(wavemaker_node
  lifecycle_msgs
  rclcpp
  rclcpp_lifecycle
  rclcpp_action
  wavemaker_interfaces
  bondcpp
  std_msgs
  mgate5101_driver
  )

add_executable(wavemaker_bridge src/wavemaker_bridge.cpp)
ament_target_dependencies(wavemaker_bridge
  rclcpp
  rclcpp_lifecycle
  rclcpp_action
  bondcpp
  wavemaker_interfaces
  std_msgs
  std_srvs
)

install(TARGETS wavemaker_node wavemaker_bridge
  DESTINATION lib/${PROJECT_NAME}
)
target_link_libraries(wavemaker_node mgate5101_driver::mgate5101)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()

  ament_add_gtest(test_indradrive_actuator
    test/test_indradrive_actuator.cpp
    src/indradrive_actuator.cpp
  )
  target_include_directories(test_indradrive_actuator PRIVATE src)
  ament_target_dependencies(test_indradrive_actuator
    rclcpp
    rclcpp_lifecycle
  )
  target_link_libraries(test_indradrive_actuator mgate5101_driver::mgate5101)

  ament_add_gtest(test_wave_trajectory
    test/test_wave_trajectory.cpp
    src/wave_math.cpp
    src/wave_trajectory.cpp
  )
  target_include_directories(test_wave_trajectory PRIVATE src)

  ament_add_gtest(test_wavemaker_node
    test/test_wavemaker_node.cpp
    src/indradrive_actuator.cpp
    src/wave_math.cpp
    src/wave_trajectory.cpp
  )
  target_include_directories(test_wavemaker_node PRIVATE src)
  ament_target_dependencies(test_wavemaker_node
    lifecycle_msgs
    rclcpp
    rclcpp_lifecycle
    rclcpp_action
    wavemaker_interfaces
    bondcpp
    std_msgs
  )
  target_link_libraries(test_wavemaker_node mgate5101_driver::mgate5101)
endif()

ament_package()