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
        Button* m_button;// todo add second button for up/down, short press could then be used to tilt blinds
        //std::shared_ptr<Button> m_master_button; // todo add master buttons up/down 
        relay* m_relay;
        shutter_position m_position{PARTIALLY_CLOSED};  //we cant know where it is at the start
        shutter_motion m_motion{STATIONARY};
        //todo: add a logger, write to a log file for debug

    public:
        shutter(uint32_t button_pin, uint32_t relay_pin1, uint32_t relay_pin2 ){ 
            uint32_t pin{button_pin};
            m_button = new Button(pin);
            m_relay = new relay(relay_pin1, relay_pin2);
            m_relay->set_off();  //make sure it is stationary at startup
        }
        shutter(std::array<uint32_t,3> pins){
            shutter(pins[0],pins[1],pins[2]);
        }
        void poll_inputs_and_update(){
            // add limit switch check here
                //if (limitswitch_bottom) 
                    //m_relay->set_off();
                    //m_position = FULLY_CLOSED

            ButtonEvent Event = m_button->poll_and_return_event();

            if(m_position== FULLY_CLOSED && Event == TRANSITION_TO_SHORT_PRESS){
                m_relay->set_positive();
                m_motion = OPENING;
            }
            if(m_position== FULLY_OPEN && Event == TRANSITION_TO_SHORT_PRESS){
                m_relay->set_negative();
                m_motion = CLOSING;
            }
        }
        ~shutter(){
            delete m_button;
            delete m_relay;
        }
};


class Shutter_system{
    //
    private:
    std::vector<shutter*> All_shutters;
    Button* SystemMasterSwitch;
    public:
    Shutter_system(std::vector<std::array<uint32_t,3>> shutter_config){
        for(int i = 0 ; i < shutter_config.size(); i++){
            shutter* shutter_ptr = new shutter(shutter_config[i]);
            All_shutters.push_back(shutter_ptr);
        }
    std::cout<< "system setup with" << All_shutters.size() << "shutters" <<  std::endl;
    std::cout << "shutter  " << "button pin   " << "relaypin1  " << "relaypin2" << std::endl;
    for(int i = 0 ; i < All_shutters.size(); i++){
        std::cout <<"     " << i+1 <<"        "<< shutter_config[i][0] <<"            "<< shutter_config[i][1] <<"      "<< shutter_config[i][2]<< std::endl;
    }
    }

};

#endif  //ifndef ROLLERSHUTTER_H
