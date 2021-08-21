#pragma once
#include "Menu.hpp"
#include "Alarms.hpp"
#include "Info.hpp"
#include "Utilities.hpp"
#include "Brightness.hpp"
#include "Debug.hpp"

class Settings: public Menu {
    public: 
        Settings():Menu("Settings"){
            resetPosOnLoadCycle(true);
            addSubDisplay("Alarm",&alarms);
            addSubDisplay("Brightness",&bm);
            addSubDisplay("Sounds",NULL);
            addSubDisplay("Utilities",&um);
            addSubDisplay("Info",&im);
            addSubDisplay("Debug", &dbg);

            crash = new MenuFunction(this,[](char* c, void* v){
                Serial.println("Crashing...");
                int* f = NULL;
                Serial.println((*f)*6);
            });
            addSubFunc("Crash", crash);
            addBackAndExit();
        };

        virtual ~Settings(){ };

    private:
        Alarms alarms;
        Debug dbg;
        MenuFunction* crash;
        Info im;
        Utilities um;
        Brightness bm;
};
