#ifndef ARDUINO_H
#define ARDUINO_H


#include <chrono>
#include <iostream>
#include <vector>
#define LOW false
#define HIGH true
#define INPUT true        // logical flag for setting to read/write mode
#define INPUT_PULLUP true //if a pullup resistor is used and hence the button read is 0 for pushed
#define OUTPUT false
#define boolean bool
#define byte uint8_t

static int global_read_counter{0};
static std::vector<bool> mock_button_input{0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
static auto start_time = std::chrono::system_clock::now();

namespace arduino
{

    byte digitalRead(byte pin, bool Switch_invert_on)
    {
        if( global_read_counter >= mock_button_input.size()){
            return 0;
        }
        auto output = mock_button_input[global_read_counter];
        global_read_counter++;
        return output;
    }
    byte digitalRead(byte pin)
    {

        global_read_counter++;
        return true;
    }

    void digitalWrite(int pin, bool value)
    {
        std::cout << "writing " << value << "to pin" << pin << std::endl;
    }

    uint32_t millis()
    {
        
        auto current_time = std::chrono::system_clock::now();
        //std::chrono::duration<int64_t> diff = current_time - start_time;
        int64_t time_since_program_start = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        uint32_t output = static_cast<uint32_t>(time_since_program_start);
        return output;
    }

    void delay(int time_ms)  
    {
        //processor blocking delay function
        auto delay_start = std::chrono::system_clock::now();
        auto current_time = delay_start;
        //wait time
        while( std::chrono::duration_cast<std::chrono::milliseconds>(current_time - delay_start).count() < time_ms ){
            current_time = std::chrono::system_clock::now();
        }
    }
    void pinMode(byte pin, bool mode)
    {
        //set the arduino pin to recieve or send
    }
    class serial
    {
    public:
        serial() {}
        void begin(int baudrate)
        {
            //set baud rate for the transmission
        }

        void print(const std::string &in)
        {
            std::cout << in << std::endl;
        }
        void print(uint8_t in)
        {
            std::cout << in << std::endl;
        }
        void println(const std::string &in)
        {
            std::cout << in << std::endl;
        }
    };

} // namespace arduino

// Class mock_read{
// public:    
//     static int global_read_counter{0};
//     static std::vector<bool> mock_button_input{0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
//     ~mock_read{}
// }

arduino::serial Serial; //global object ala std::cout

#endif