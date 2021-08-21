#include "include/TimeDisplay.h"
#include "include/Settings.hpp"
//#include <StreamString.h>

extern Settings mainMenu;

void TimeDisplay::updateDisplay(){
    if (displayUpdate_m.isReady()){
        display();
    }
}

void TimeDisplay::onLoad(){
    display_mp->clearDisplay();
    updateData();
    display();
}

void TimeDisplay::update(){
    updateData();
    updateDisplay();
}

void TimeDisplay::updateData(){
    t = TimeManager::now();
    colon = (t->tm_sec % 2);
}

void TimeDisplay::onButton(uint8_t now){
    if (now != 0)
        displayManager_mp->addEnd(&mainMenu);
};

void TimeDisplay::display(){
    display_mp->clearDisplay();
    display_mp->setTextColor(WHITE);
    display_mp->setTextSize (6);
    byte hour = t->tm_hour > 12? (t->tm_hour-12):t->tm_hour;
    hour = (hour == 0)?12:hour;
    display_mp->setCursor ((hour >= 10) ? 42 : 17, 0);
    display_mp->print(colon?":":" ");
    display_mp->setFont (&FreeSans24pt7b);
    display_mp->setTextSize(1);
    display_mp->setCursor(0, 37);
    display_mp->print(String(hour));
    display_mp->print (" ");
    if (t->tm_min < 10)
        display_mp->print("0");
    display_mp->print(String(t->tm_min));
    display_mp->setFont ();
    // screen.print ((alarmSkipTime != -1) ? "Mute" : "");
    //screen.print (( alarmToday) ? "Alarm" : "");
    display_mp->setTextSize(2);
    display_mp->setCursor(0, 45);
    //StringStream ss;
    //ss << "foo";
    display_mp->print(String(t->tm_mday));
    display_mp->print("/");
    display_mp->print(String(t->tm_mon + 1));
    display_mp->print("/");
    display_mp->print(String(t->tm_year + 1900));
    display_mp->display();
}