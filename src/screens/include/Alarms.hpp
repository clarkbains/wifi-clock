#pragma once
#include "Menu.hpp"
#include "AlarmsEditor.hpp"
#include "../../lib/include/AlarmManager.hpp"

class Alarms: public Menu {
    public: 
        Alarms():Menu("Alarms"){
            r = &AlarmManagerInstance::instance();
            currentNumDisplayed = 0;
            resetPosOnLoadCycle(true);
        };

    private:
        virtual void onUpdate(){
            if (currentNumDisplayed != r->getNumAlarms()){
                Serial.println("Loading Alarms\n");

                //Menu
                clear();
                //                Serial.println("Loading Alarms\n");

                for (int16_t i = currentNumDisplayed; i>=0; i--){
                    delete alrms[i];
                }
                currentNumDisplayed = r->getNumAlarms();
                for (uint8 i = 0; i < currentNumDisplayed; i++){
                    alrms[i] = new AlarmEditor(r->getWakeAlarm(i));
                    addSubDisplay((char*)r->getWakeAlarm(i)->getName(), alrms[i]);
                }
                
                addSubFunc("new", new MenuFunction(this,[](char* c, void* v){
                    auto self = (Alarms*) v;
                    AlarmManagerInstance::instance().addAlarm(new WakeAlarm(WakeAlarm::weekDays(),0,0));
                }));
                addBackAndExit();
            }
        }
        AlarmEditor* alrms[AlarmManager::MAX_ALARMS];
        AlarmManager* r;
        uint8 currentNumDisplayed;
};