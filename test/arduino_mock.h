#pragma once

#ifndef ARDUINO_H
#define ARDUINO_H

//std dependencies
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

// preprocessor definitions used in the arduino header
#define LOW false
#define HIGH true
#define INPUT true        // logical flag for setting to read/write mode
#define INPUT_PULLUP true //if a pullup resistor is used and hence the button read is 0 for pushed
#define OUTPUT false
#define boolean bool
#define byte uint8_t


//put all the functions in the namespace which allows the 
namespace arduino
{
    static int global_read_counter{0};
    static int global_read_step{10};
    static std::vector<bool> mock_button_input; 
    //static auto start_time = std::chrono::system_clock::now();

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
        return global_read_counter*global_read_step;
        // auto current_time = std::chrono::system_clock::now();
        // //std::chrono::duration<int64_t> diff = current_time - start_time;
        // int64_t time_since_program_start = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        // uint32_t output = static_cast<uint32_t>(time_since_program_start);
        // return output;
    }

    void delay(int time_ms)  
    {
        //processor blocking delay function
        std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
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


serial Serial; //global object in arduino namespace ala std::cout
} // namespace arduino

// Class mock_read{
// public:    
//     static int global_read_counter{0};
//     static std::vector<bool> mock_button_input{0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
//     ~mock_read{}
// }

#endif