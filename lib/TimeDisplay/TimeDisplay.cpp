#include "TimeDisplay.h"


using namespace radio_drivers; 

RadioTime::RadioTime(uint8_t in_hours, uint8_t in_min, bool is_am) : minutes{in_min}, 
    hours{in_hours}, am{is_am}
{
    //check to see that the times are in bounds if not report an 
    //error and set to zero 
    if (minutes > 59 || hours > 12 || hours == 0) 
    {
        LOG_ERROR("time at init was invalid"); 
        minutes = 0; 
        hours = 0; 
    }
}

void RadioTime::add_time(uint8_t h, uint8_t m)
{
    uint8_t adjusted_hours = h; 
    uint8_t current_mil_time = convert_to_military(hours,am); 

    minutes += m; 
    if (minutes > 59) 
    {
        adjusted_hours += minutes / 60; //number of hours to add 
        minutes %= 60; 
    }

    current_mil_time = (adjusted_hours + current_mil_time) % 24; 
    auto new_time = convert_to_standard(current_mil_time); 
    hours = new_time.first; 
    am = new_time.second;
}

void RadioTime::sub_time(uint8_t h, uint8_t m)
{
    //first solve for the minutes 
    int min_result = minutes - m; 
    uint8_t sub_hours = std::abs(min_result - 60) / 60; 
    minutes = std::abs(min_result + 60) % 60; 

    //number of hours to subtract 
    sub_hours += h; 

    //need signed in this case 
    int8_t current_military_hour = convert_to_military(hours,am); 
    current_military_hour -= sub_hours; 
    auto result = convert_to_standard((current_military_hour + 24) % 24); 
    hours = result.first; 
    am = result.second; 
}

std::pair<uint8_t, bool> RadioTime::convert_to_standard(uint8_t h) 
{
    //log if not in the right range
    if ( h > 23) 
    {
        LOG_ERROR("cannot convert back to standard time : ", h); 
        return {0,0}; 
    }
    uint8_t result_hour; 
    bool temp_am; 

    if (h < 12) 
    {
        // should be am 
        temp_am = true; 
        result_hour = (h != 0) ? h : 12; //return 12 for am 
    }
    else 
    {
        temp_am = false; 
        result_hour = (h - 12 != 0) ? h - 12 : 12; //dont subtract for 12 pm
    }
    return {result_hour,temp_am}; 
}

uint8_t RadioTime::convert_to_military(uint8_t h, bool is_am)
{
    //log invalid entries 
    if (h > 12 || h < 1)
    {
        LOG_ERROR("invalid hour entered : ", h);
        LOG_ERROR("when converting to military time");  
        return 0; 
    }
    if (h == 12) 
    {
        return !is_am * 12; 
    }
    else 
    {
        return h + is_am * 12; 
    }
}

//getters 
uint8_t RadioTime::get_mins() const { return minutes; }
uint8_t RadioTime::get_hours() const { return hours; }
bool RadioTime::is_am() const {return am; } 

bool RadioTime::set_time(uint8_t in_hours, uint8_t in_min, bool is_am)
{
    if (in_min > 59 || in_hours > 12 || in_hours == 0) 
    {
        LOG_ERROR("invalid time attempt"); 
        return false; 
    }
    else 
    {
        minutes = in_min; 
        hours = in_hours; 
        am = is_am; 
        return true; 
    }
}


SevenSegment::SevenSegment(uint8_t in_clk_pin, uint8_t in_data_pin, RadioTime& in_ref) : 
    time_ref{in_ref}, 
    clk_pin{in_clk_pin}, 
    data_pin{in_data_pin}, 
    TM1637Display(in_clk_pin,in_data_pin)
{}

void SevenSegment::Update() 
{
    showNumberDecEx(time_ref.get_hours() * 100 + time_ref.get_mins(),0b11100000); 
}
