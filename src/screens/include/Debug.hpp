#pragma once
#include "Menu.hpp"
#include "../../lib/include/AlarmManager.hpp"
class Debug: public Menu {

    public: 
        Debug():Menu("Debug"), dumpAlarms((void*) this, Debug::dumpA){
            addSubFunc("D. ALRM", &dumpAlarms);
            addBackAndExit();
            resetPosOnLoadCycle(true);
        };
    private:
     MenuFunction dumpAlarms;
     static void dumpA(char* c, void* v){
        AlarmManagerInstance::instance().dump();
    }    

    
};