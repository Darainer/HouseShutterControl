#include "arduino.h"
#include "button.hpp"

enum relay_status{
  OFF,
  POSITIVE,
  NEGATIVE
}

class relay{
    private:
    int32_t m_pin1{-1};
    int32_t m_pin2{0};
    bool m_pin_invert{0};
    relay_status status{OFF};
    public:
    relay(int32_t pin1, int32_t pin2, bool invert){
        m_pin_invert = invert; //if the terminals are reversed
        if(!invert){
            m_pin1 = pin1;
            m_pin2 = pin2;
        }
        else(){  //switch pins to use the same methods
            m_pin1 = pin2;
            m_pin2 = pin1;
        }
    }
    void set_positive(){
        arduino::digitalWrite(m_pin1,true);
        arduino::digitalWrite(m_pin2,false);
        status = POSITIVE;
    }
    void set_negative(){
        arduino::digitalWrite(m_pin1,false);
        arduino::digitalWrite(m_pin2,true);
        status = NEGATIVE;
    }
    void set_off(){
        arduino::digitalWrite(m_pin1,false);
        arduino::digitalWrite(m_pin2,false);
        status = OFF;
    }
    relay_status read_current_state(){
        return status;
    }
};

class shutter{
    private:
        Button m_button;// todo: need to call constructor with the pin 
        std::shared_ptr<Button> m_master_button;
        relay m_relay;
    
    public:
        shutter(int32_t button_pin, std::shared_ptr<Button> master_button ){ 
        }

};