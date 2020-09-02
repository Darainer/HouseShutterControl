#include "../rollershutter.hpp"
#include "../googletest/googletest/include/gtest/gtest.h"

// Tests that button instantiates without error
TEST(Instanciation, HandlesZeroInput) {
  // instanciation
  Button masterswitch(1);
  std::shared_ptr<Button> master;
  int32_t pin_button{2};
  ASSERT_NO_THROW(shutter myshutter(pin_button, master));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}