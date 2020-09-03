#ifndef ROLLERSHUTTER_H
#define ROLLERSHUTTER_H

#ifdef unit_test
  //#include "arduino_mock.h"
  using namespace arduino;
#else
  #include "arduino.h"
#endif

#include "button.hpp"

enum relay_status{
  OFF,
  POSITIVE,
  NEGATIVE
};

class relay{
    private:
    uint32_t m_pin1{0};
    uint32_t m_pin2{1};
    bool m_pin_invert{0};
    relay_status status{OFF};
    public:
        relay(uint32_t pin1, uint32_t pin2) {
            m_pin1 = pin1;
            m_pin2 = pin2;
        }
        relay(uint32_t pin1, uint32_t pin2, bool invert) {
            m_pin_invert = invert; //if the terminals are reversed
            if(!invert){
                m_pin1 = pin1;
                m_pin2 = pin2;
            }
            else{  //switch pins to use the same methods
                m_pin1 = pin2;
                m_pin2 = pin1;
            }
        }
    void set_positive(){
        digitalWrite(m_pin1,true);
        digitalWrite(m_pin2,false);
        status = POSITIVE;
    }
    void set_negative(){
        digitalWrite(m_pin1,false);
        digitalWrite(m_pin2,true);
        status = NEGATIVE;
    }
    void set_off(){
        digitalWrite(m_pin1,false);
        digitalWrite(m_pin2,false);
        status = OFF;
    }
    relay_status read_current_state(){
        return status;
    }
};

enum shutter_position{
  FULLY_CLOSED,
  PARTIALLY_CLOSED,
  FULLY_OPEN,
};
enum shutter_motion{
  OPENING,
  CLOSING,
  STATIONARY,
};

class shutter{
    private:
        Button* m_button;// todo: need to call constructor with the pin 
        // std::shared_ptr<Button> m_master_button;  todo add master button 
        relay* m_relay;
        shutter_position m_position{PARTIALLY_CLOSED};  //we cant know where it is at the start
        shutter_motion m_motion{STATIONARY};

    public:
        shutter(uint32_t button_pin, uint32_t relay_pin1, uint32_t relay_pin2 ){ 
            uint32_t pin{button_pin};
            m_button = new Button(pin);
            m_relay = new relay(relay_pin1, relay_pin2);
            m_relay->set_off();
        }
        void poll_inputs_and_update(){
            // add limit switch check here eg...
                //if (limitswitch) 
                    //m_relay->set_off();
            ButtonEvent Event = m_button->poll_and_return_event();

            if(m_position== FULLY_CLOSED && Event == ButtonEvent::TRANSITION_TO_SHORT_PRESS){
                m_relay->set_positive();
                m_motion = OPENING;
            }
        }
        ~shutter(){
            delete m_button;
            delete m_relay;
        }
};

#endif  //ifndef ROLLERSHUTTER_H
