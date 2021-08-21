#pragma once
#include "../../include/Screen.hpp"
#include "../../../utilities/include/DelayedEvent.h"
#include "../../include/DisplayManager.h"
class Alert: public Screen {
    public: 
        Alert(uint16 t=5000):de(t){
        };
        
    protected:
        DelayedEvent de;
        
        virtual void onLoad(){
            de.start();
        };
        virtual void update(){
            if (de.shouldRun()){
                displayManager_mp->popStack();
                return;
            }
        }
};
