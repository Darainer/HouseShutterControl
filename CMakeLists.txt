cmake_minimum_required(VERSION 3.10)
PROJECT(ROLLERSHUTTER)
set(CMAKE_CXX_STANDARD 11) 

ADD_EXECUTABLE(roller main.cpp arduino.h button.hpp)

add_subdirectory(googletest)
add_executable(testroller test/button_test.cpp arduino.h button.hpp)
target_link_libraries(testroller gtest_main)

