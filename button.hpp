#ifndef BUTTON_H
#define BUTTON_H

#include "arduino.h"

using namespace arduino;

enum ButtonEvent
{
  NO_TRANSITION_EVENT,
  SHORT_PRESS,
  LONG_PRESS
};

class Button
{
private:
  uint32_t m_uCPin;
  ButtonEvent Event;
  bool m_isPressed{true};
  bool m_LastReadState{false};
  uint32_t m_debouncetime_ms{25};
  uint32_t m_last_statechange_ms{0};
  uint32_t short_press_ms{500};
  uint32_t long_press_ms{1000};
  bool m_switch_invert{false};

public:
  Button(uint32_t pin)
  {
    m_uCPin = pin;
    m_last_statechange_ms = millis();
  }
  void read_and_debounce_button()
  {
    bool raw_switch_reading = digitalRead(m_uCPin, m_switch_invert);
    uint32_t current_time_in_ms = millis();

    // If the switch changed, due to noise or pressing
    if (raw_switch_reading != m_LastReadState)
    {
      m_last_statechange_ms = millis(); //reset the debouncing timer
    }
    uint32_t time_in_current_state = (current_time_in_ms - m_last_statechange_ms);
    if (time_in_current_state > m_debouncetime_ms)
    {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state
      if (raw_switch_reading != m_isPressed)
      {
        m_isPressed = raw_switch_reading;
      }
    }
  }
  ButtonEvent check_button_event()
  {
    // if the switch is not pressed, no event
    if (m_isPressed == 0)
    {
      if (Event != NO_TRANSITION_EVENT)
      {
        Event = NO_TRANSITION_EVENT;
      }
    }

    uint32_t current_time_in_ms = millis();
    int time_in_current_state = (current_time_in_ms - m_last_statechange_ms);

    if (Event != LONG_PRESS && m_isPressed && time_in_current_state > long_press_ms)
    {
      Event = LONG_PRESS;
    }
    else if (Event != SHORT_PRESS && time_in_current_state > short_press_ms){
      Event = SHORT_PRESS;
    }
    else{  // if we already reported the button press, then longer pushing should have no effect
      Event = NO_TRANSITION_EVENT;
    }
    return Event;
  }

  ButtonEvent poll_and_return_event()
  {
    read_and_debounce_button();
    ButtonEvent current_event = check_button_event();
    return current_event;
  }
};
#endif