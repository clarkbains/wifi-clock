#pragma once
#include "Menu.hpp"
class Brightness: public Menu {
    public: 
        Brightness():Menu("Brightness"){
            
            for (uint8 i = 0; i < MAX_BUFFER_LEN; i++){
                brightnessStr_a[i] = 0;
            }

            addSpecialEntry(brightnessStr_a, NOOP);
            
            MenuFunction* down = new MenuFunction(this,[](char* c, void* v){
                auto self = (Brightness*) v;
                self->display_mp->dimmer();
            });
            
            MenuFunction* up = new MenuFunction(this,[](char* c, void* v){
                auto self = (Brightness*) v;
                self->display_mp->brighter();

            });

            addSubFunc("Down", down);
            addSubFunc("Up", up);
            addBackAndExit();
            resetPosOnLoadCycle(true);
        };

        ~Brightness(){
        //    delete up;
        //    delete down;
        }
    
    private:
        static const uint8 MAX_BUFFER_LEN = 14;
        char brightnessStr_a[MAX_BUFFER_LEN];
        MenuFunction* up;
        MenuFunction* down;

        virtual void onUpdate(){
            snprintf(brightnessStr_a,MAX_BUFFER_LEN, "Cur: %hhu/%hhu", 
                (uint8)(display_mp->getCurrentBrightness()+1)
                ,SSD1306Brightness::NUM_BRIGHTNESS_STEPS);
        };
    
};