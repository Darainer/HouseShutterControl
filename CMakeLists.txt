cmake_minimum_required(VERSION 3.10)
PROJECT(ROLLERSHUTTER)
set(CMAKE_CXX_STANDARD 11) 

ADD_EXECUTABLE(roller main.cpp arduino.h button.hpp)

add_subdirectory(googletest)

add_executable(testbutton test/button_test.cpp arduino.h button.hpp)
target_link_libraries(testbutton gtest_main)

add_executable(testroller test/roller_shutter_test.cpp arduino.h rollershutter.hpp)
target_link_libraries(testroller gtest_main)

