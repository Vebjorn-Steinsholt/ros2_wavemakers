cmake_minimum_required(VERSION 3.15)
project(mgate5101_driver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(mgate5101
    src/modbus_tcp.cpp
    src/process_image.cpp
    src/mgate_driver.cpp
    src/indradrive.cpp
)
target_include_directories(mgate5101 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(WIN32)
    target_link_libraries(mgate5101 PUBLIC ws2_32)
    target_compile_definitions(mgate5101 PUBLIC _WIN32_WINNT=0x0601 NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

if(MSVC)
    target_compile_options(mgate5101 PRIVATE /W4 /permissive-)
else()
    target_compile_options(mgate5101 PRIVATE -Wall -Wextra)
endif()

add_executable(mgate_example examples/example_main.cpp)
target_link_libraries(mgate_example PRIVATE mgate5101)

# test_mapping is self-contained.
# test_driver needs the stub: python tests/stub_server.py  (listens on 127.0.0.1:1502)
enable_testing()

add_executable(test_mapping tests/test_mapping.cpp)
target_link_libraries(test_mapping PRIVATE mgate5101)
add_test(NAME mapping COMMAND test_mapping)

add_executable(test_driver tests/test_driver.cpp)
target_link_libraries(test_driver PRIVATE mgate5101)
