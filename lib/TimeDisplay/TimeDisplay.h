#pragma once
#include <Arduino.h> 
#include "TM1637Display.h"
#include "DebugLog.h"


namespace radio_drivers
{

class RadioTime
{
    public: 
    RadioTime(uint8_t in_hours, uint8_t in_min, bool is_am); 

    void add_time(uint8_t h, uint8_t m); 
    void sub_time(uint8_t h, uint8_t m); 

    //getters 
    uint8_t get_mins() const; 
    uint8_t get_hours() const;
    bool is_am() const;  

    //setters 
    bool set_time(uint8_t in_hours, uint8_t in_min, bool is_am); 

    private: 
    uint8_t convert_to_military(uint8_t h, bool is_am); //convert hours to military time
    std::pair<uint8_t, bool>  convert_to_standard(uint8_t h); 
    uint8_t minutes; 
    uint8_t hours; 
    bool am; 
}; 

//focused on diplaying seven seg info as time and includes some extra utilites to do so 
class SevenSegment : public TM1637Display
{
    public: 
    SevenSegment(uint8_t in_clk_pin, uint8_t in_data_pin, RadioTime& in_ref); 
    void Update(); //update display based on RadioTime reference


    private: 
    RadioTime& time_ref; //reference time to use for udpating the display 
    uint8_t clk_pin; 
    uint8_t data_pin; 
}; 

} // radio drivers