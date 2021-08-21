#pragma once
#include "../../include/Screen.hpp"
#include "../../../utilities/include/DelayedEvent.h"
#include "../../include/DisplayManager.h"
class ReccurringAlarm: public Screen {
    public: 
        ReccurringAlarm():de(2000),re(10000){
            t = 0;
        };

        
    private:
        uint8 t;
        DelayedEvent de;
        RepeatingEvent re;

        
        virtual void onLoad(){
            display_mp->clearDisplay();
            display_mp->setTextSize(2);
            display_mp->println("1 sec");
            display_mp->println("Alarm");
            display_mp->println(":)");
            display_mp->println(t);
            display_mp->display();
            de.start();
        };

        virtual void update(){
            if (de.shouldRun()){
                displayManager_mp->popStack();
                return;
            }
        }

        virtual void onHeartbeat(){
            if (displayManager_mp->isActiveScreen(this)) return;
            if (re.isReady()){
                t++;
                displayManager_mp->addEnd(this);
            }
        }
};
