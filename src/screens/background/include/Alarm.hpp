#pragma once
#include "Alert.hpp"
#include "../../../utilities/include/DelayedEvent.h"
#include "../../include/DisplayManager.h"
#include "../../../hw/include/BuzzerManager.h"
#include "../../../lib/include/TimeManager.hpp"
#include "../../../lib/include/AlarmManager.hpp"


class AlarmProcessor: public Alert {
    public: 
        AlarmProcessor():Alert(10000){
            a = 0;
            AlarmManagerInstance::instance().addAlarm(new WakeAlarm(WakeAlarm::weekDays(), 20, 37, "Wk Up"));
        }

    private:
        char* t;
        char a;
        virtual void onLoad(){
            BuzzerManager::instance().start();
            display_mp->clearDisplay();
            display_mp->setTextSize(2);
            display_mp->println("Alarm");
            display_mp->println(t);
            display_mp->println(":)");
            display_mp->display();
            Alert::onLoad();
        };
        
        virtual void onUnload(){
            BuzzerManager::instance().stop();
        }
        
        virtual void onButton(uint8_t now){
            displayManager_mp->popStack();
        };

        virtual void update(){
            Alert::update();
        }

        virtual uint8 getPriority()
        { return 10; };


        virtual void onHeartbeat(){
            if (displayManager_mp->isActiveScreen(this)) return;
            WakeAlarm *wa = AlarmManagerInstance::instance().getActiveAlarm();
            if (wa == NULL){return;}
            t = wa->getTOD();
            displayManager_mp->addEnd(this);
        }
};
