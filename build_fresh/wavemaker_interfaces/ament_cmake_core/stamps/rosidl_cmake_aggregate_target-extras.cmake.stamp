# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target wavemaker_interfaces::wavemaker_interfaces
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${wavemaker_interfaces_TARGETS}.
if(wavemaker_interfaces_TARGETS AND NOT TARGET wavemaker_interfaces::wavemaker_interfaces)
  add_library(wavemaker_interfaces::wavemaker_interfaces INTERFACE IMPORTED)
  set_target_properties(wavemaker_interfaces::wavemaker_interfaces PROPERTIES
    INTERFACE_LINK_LIBRARIES "${wavemaker_interfaces_TARGETS}")
endif()
