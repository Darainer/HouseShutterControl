#include "../button.hpp"
#include "../googletest/googletest/include/gtest/gtest.h"


// Tests that Foo does Xyz.
TEST(OneRead, HandlesZeroInput) {
  // instanciation
  Button myButton(1);
  ASSERT_NO_THROW(myButton.read_and_debounce_button());

}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}