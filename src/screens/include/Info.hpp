#pragma once
#include "Screen.hpp"

class Info: public Screen {   
    private:
        virtual void onLoad(){
            display_mp->clearDisplay();
            display_mp->setTextColor(WHITE);
            display_mp->setCursor(0,0);
            display_mp->setTextSize(2);
            display_mp->println("Clark Bains");
            display_mp->setTextSize(1);
            display_mp->println("613-250-2952");
            display_mp->println("projects@cbains.ca");
            display_mp->println();
            display_mp->println("Compiled On:");
            display_mp->println(__DATE__);
            display_mp->println(__TIME__);
            display_mp->display();
        };

        virtual void onButton(uint8_t now, uint8_t old){
            displayManager_mp->popStack();
        };
};