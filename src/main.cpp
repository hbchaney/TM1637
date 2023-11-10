#include <Arduino.h>
#include "TM1637Display.h"
#include "TimeDisplay.h"

// TM1637Display disp(16,17,75); 
radio_drivers::RadioTime rad_time(10,16,true); 
radio_drivers::SevenSegment disp(16,17,rad_time); 
int count = 0; 

void setup() 
{
    disp.setBrightness(1); 
}


void loop() 
{
    disp.Update(); 
    sleep_ms(200); 
    rad_time.add_time(0,2); 
}