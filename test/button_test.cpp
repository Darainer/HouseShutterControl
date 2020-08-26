#include "../button.hpp"
#include "../googletest/googletest/include/gtest/gtest.h"

void fixture_shortbuttonPress(){
  std::vector<int> a_short_press{0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0};
}

// Tests that Foo does Xyz.
TEST(Instanciation, HandlesZeroInput) {
  // instanciation
  ASSERT_NO_THROW(Button myButton(1));
}

// Tests that Foo does Xyz.
TEST(call_read_and_debounce, HandlesZeroInput) {
  // instanciation
  Button myButton(1);
  ASSERT_NO_THROW(myButton.poll_and_return_event());
}


TEST(shortpress, HandlesZeroInput) {
  // instanciation
  Button myButton(1);

  mock_button_input= {0,1,1,1,1,0,0,0,0};
  std::vector<ButtonEvent> expected_event = {NO_TRANSITION,NO_TRANSITION,NO_TRANSITION,NO_TRANSITION,TRANSITION_TO_SHORT_PRESS,NO_TRANSITION, NO_TRANSITION, NO_TRANSITION, TRANSITION_TO_OFF};

  while(global_read_counter<mock_button_input.size()){
    ASSERT_EQ(myButton.poll_and_return_event(), expected_event[global_read_counter]);
  }

}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}