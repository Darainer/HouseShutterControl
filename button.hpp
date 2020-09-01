#ifndef BUTTON_H
#define BUTTON_H


#include "arduino.h"

using namespace arduino;

enum ButtonEvent
{
  TRANSITION_TO_OFF,
  TRANSITION_TO_SHORT_PRESS,
  TRANSITION_TO_LONG_PRESS,
  NO_TRANSITION
};

enum ButtonStatus
{
  NOT_PRESSED,
  SHORT_PRESS,
  LONG_PRESS
};

class Button
{
private:
  uint32_t m_uCPin;
  ButtonStatus m_ButtonStatus_previous{NOT_PRESSED};
  ButtonStatus m_ButtonStatus{NOT_PRESSED};
  bool m_SwitchisPressed{true};
  bool m_LastReadState{false};
  uint32_t m_debouncetime_ms{25};
  uint32_t m_last_statechange_ms{0};
  uint32_t short_press_ms{25};
  uint32_t long_press_ms{1000};
  bool m_switch_invert{false};

  void read_and_debounce_switch()
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
        m_SwitchisPressed = raw_switch_reading;
    }
    m_LastReadState = raw_switch_reading;
  }

  void updateButtonState()
  {
    //simple state machine for the button
    m_ButtonStatus_previous = m_ButtonStatus;
    uint32_t current_time_in_ms = millis();
    int time_in_current_state = (current_time_in_ms - m_last_statechange_ms);

    if (m_SwitchisPressed && time_in_current_state > long_press_ms)
    {
      m_ButtonStatus = LONG_PRESS;
    }
    else if (m_SwitchisPressed && time_in_current_state > short_press_ms){
      m_ButtonStatus = SHORT_PRESS;
    }
    else if(!m_SwitchisPressed && time_in_current_state > m_debouncetime_ms){  //transition to off
      //
      m_ButtonStatus = NOT_PRESSED;
    }
  }

  ButtonEvent checkForButtonEvent(){
    ButtonEvent event{NO_TRANSITION};
    if(m_ButtonStatus != m_ButtonStatus_previous ){
      if(m_ButtonStatus_previous == NOT_PRESSED && m_ButtonStatus == SHORT_PRESS){
          event = TRANSITION_TO_SHORT_PRESS;
      }
      else if(m_ButtonStatus_previous == SHORT_PRESS && m_ButtonStatus == LONG_PRESS){
          event = TRANSITION_TO_LONG_PRESS;
      }
      else if( (m_ButtonStatus_previous == SHORT_PRESS || m_ButtonStatus_previous == LONG_PRESS) && m_ButtonStatus == NOT_PRESSED){
          event = TRANSITION_TO_OFF;
      }
    }
    return event;
  }

public:
  Button(uint32_t pin)
  {
    m_uCPin = pin;
    m_last_statechange_ms = millis();
  }

  ButtonEvent poll_and_return_event()
  {
    read_and_debounce_switch();
    updateButtonState();
    return checkForButtonEvent();
  }
};
#endif