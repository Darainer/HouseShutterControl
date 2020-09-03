#include "../googletest/googletest/include/gtest/gtest.h"
#include "arduino_mock.h"
#define unit_test
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}