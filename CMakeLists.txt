cmake_minimum_required(VERSION 3.10)
PROJECT(ROLLERSHUTTER)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_) 


add_subdirectory(googletest)

add_executable(testbutton test/button_test.cpp test/arduino_mock.h button.hpp)
target_link_libraries(testbutton gtest_main)
target_compile_definitions(testbutton PUBLIC -DArduino_mock_unit_test)


add_executable(testroller test/roller_shutter_test.cpp test/arduino_mock.h rollershutter.hpp)
target_link_libraries(testroller gtest_main)
target_compile_definitions(testroller PUBLIC -DArduino_mock_unit_test)

