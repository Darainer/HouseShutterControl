#include "../googletest/googletest/include/gtest/gtest.h"
#include "arduino_mock.h"
// #define unit_test
#include "../button.hpp"

void fixture_shortbuttonPress(){
  std::vector<int> a_short_press{0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0};
}

// Tests that button instantiates without error
TEST(Instanciation, HandlesZeroInput) {
  // instanciation
  ASSERT_NO_THROW(Button myButton(1));
}

// Tests that main polling function is not throwing
TEST(call_read_and_debounce, HandlesZeroInput) {
  // instanciation
  Button myButton(1);
  ASSERT_NO_THROW(myButton.poll_and_return_event());
}

// test that a short button push event is correctly handled by the button class 
TEST(shortpress, HandlesZeroInput) {
  // instanciation
  Button myButton(1,25,25,100);
  global_read_step = 10;  //use 10ms for this test
  mock_button_input= {0,1,1,1,1,0,0,0,0};
  std::vector<ButtonEvent> expected_event = {NO_TRANSITION,NO_TRANSITION,NO_TRANSITION,NO_TRANSITION,TRANSITION_TO_SHORT_PRESS,NO_TRANSITION, NO_TRANSITION, NO_TRANSITION, TRANSITION_TO_OFF};
  int read_counter{0};
  while(read_counter<mock_button_input.size()){
    ASSERT_EQ(myButton.poll_and_return_event(), expected_event[read_counter]);
    //std::cout << read_counter<< std::endl;
    read_counter++;
  }

}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}