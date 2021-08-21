
#pragma once
#include "Screen.hpp"
#include "time.h"
#include "../../lib/include/TimeManager.hpp"
#include "../../utilities/include/RepeatingEvent.h"

class TimeDisplay: public Screen
{
public:
    TimeDisplay():colon(false),lastS(0),displayUpdate_m(1000){}


private:
    void display();
    void updateData();
    void updateDisplay();
    long lastS;
    RepeatingEvent displayUpdate_m;
    bool colon;
    struct tm* t;
    virtual void update();
    virtual void onLoad();
   //virtual void onUnload(){};
   virtual void onButton(uint8_t);
   virtual uint8 getPriority()
   { return 255; };

};
