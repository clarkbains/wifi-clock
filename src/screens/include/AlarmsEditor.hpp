#pragma once
#include "Menu.hpp"
#include "TimeEditor.hpp"
#include "../../lib/include/AlarmManager.hpp"

class AlarmEditor: public Menu {
    public: 
        AlarmEditor(WakeAlarm* w):Menu( (char*) w->getName()), wa(w), te(w){
            addSpecialEntry(w->getTOD(), NOOP);
            addSpecialEntry(w->getDOW(), NOOP);
            addSubDisplay("edit", &te);
            addBackAndExit();
            resetPosOnLoadCycle(true);
        };
    private:

        WakeAlarm* wa;
        TimeEditor te;
    
};