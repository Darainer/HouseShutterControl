#include "../googletest/googletest/include/gtest/gtest.h"
#include "arduino_mock.h"
#define unit_test
#include "../rollershutter.hpp"

// Tests that button instantiates without error
TEST(Instanciation, HandlesZeroInput) {
  // instanciation
  uint32_t pin_button{2};
  ASSERT_NO_THROW(shutter myshutter(pin_button, 1,2));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}