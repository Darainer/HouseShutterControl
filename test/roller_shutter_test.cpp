#include "../googletest/googletest/include/gtest/gtest.h"
#include "arduino_mock.h"
// #define Arduino_mock_unit_test
#include "../rollershutter.hpp"

// Tests that button instantiates without error
TEST(Instanciation, HandlesZeroInput) {
  // instanciation
  uint32_t pin_button{2};
  uint32_t relay_pin1{1}, relay_pin2{2};
  ASSERT_NO_THROW(shutter myshutter(pin_button,relay_pin1,relay_pin2));
}

TEST(update, HandlesZeroInput) {
  // test update function call
  uint32_t pin_button{2};
  uint32_t relay_pin1{1}, relay_pin2{2};
  shutter myshutter(pin_button,relay_pin1,relay_pin2);
  ASSERT_NO_THROW(myshutter.poll_inputs_and_update());

}

TEST(shutter_system_instantiation, HandlesZeroInput) {
  // test update function call
  uint32_t pin_button{3};
  uint32_t relay_pin1{1}, relay_pin2{2};
  std::vector<std::array<uint32_t,3>> input_shutter{{pin_button,relay_pin1,relay_pin2}, {4,5,6}};

  ASSERT_NO_THROW(Shutter_system myshutters(input_shutter));

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}