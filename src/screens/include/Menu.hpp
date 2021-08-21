#pragma once
#include "Screen.hpp"
#include "DisplayManager.h"
enum EntryAttributes {EXIT,NOOP,BACK,NORMAL};
#define NO_SELECTION_YET -1
typedef void (*DisplayExec_t)(char*, void*);


class MenuFunction {
    public:
        MenuFunction(void* d, void(f)(char*, void*)):data(d),onSelect(f){
        };
        void run(char* c){
            onSelect(c, data);
        }
    private:
        void (*onSelect)(char*, void*);
        void* data;        
};

class Menu: public Screen{
    public:
        Menu(char* title = NULL);
        virtual ~Menu(){
            clear();
            if (hasTitle_m){
                delete text_map[0];
            }
        };

    protected:
        void addSubDisplay(char* t, Screen* d){
            displays_map[numMenuOptions_m]  = d;
            attr_ma[numMenuOptions_m]       = NORMAL;
            funcs_ma[numMenuOptions_m]      = NULL;
            text_map[numMenuOptions_m++]    = t;
        };
        void addSubFunc(char* t, MenuFunction* mf){
            displays_map[numMenuOptions_m]  = NULL;
            attr_ma[numMenuOptions_m]       = NORMAL;
            funcs_ma[numMenuOptions_m]      = mf;
            text_map[numMenuOptions_m++]    = t;
        };
        void addSpecialEntry(char* t, EntryAttributes a = NORMAL){
            displays_map[numMenuOptions_m]  = NULL;
            attr_ma[numMenuOptions_m]       = a;
            funcs_ma[numMenuOptions_m]      = NULL;
            text_map[numMenuOptions_m++]    = t; 
        }
        void resetPosOnLoadCycle(bool b){
            resetMenuPosOnLoadCycle_m = b;
        };

        void addBackAndExit(){
            addSpecialEntry("BACK", BACK);
            addSpecialEntry("EXIT", EXIT);
        };

        void clear(){
            for (int i = numMenuOptions_m-1; i>=0; i--)
                if (funcs_ma[i] != NULL)
                    delete funcs_ma[i];
            selectedOption_m = 0;
            numMenuOptions_m = hasTitle_m?1:0;
        }

    private:
        virtual void onLoad(){
            display_mp->setFont();
            display_mp->setTextSize(2);
            display_mp->setTextWrap(false);
            
            if (selectedOption_m == NO_SELECTION_YET){
                changeMenuPos(+1);
            }
            update();
        };

 
        virtual void onUnload(){
           if (resetMenuPosOnLoadCycle_m){
            selectedOption_m = 0; 
            changeMenuPos(+1);
           }
        }
        virtual void onUpdate(){};
        virtual void update();

        virtual void onButton(uint8_t, uint8_t);

        static const uint8_t MAX_MENU_OPTS = 10;
        static const uint8_t MAX_ON_DISPLAY = 4;
        static const uint8_t TEXT_HEIGHT = 16;

        uint8_t numMenuOptions_m;
        int16_t selectedOption_m;
        uint8_t startingIndex_m;
        Screen* displays_map[MAX_MENU_OPTS];
        char*    text_map[MAX_MENU_OPTS];
        EntryAttributes attr_ma[MAX_MENU_OPTS];
        MenuFunction*   funcs_ma[MAX_MENU_OPTS];
        bool resetMenuPosOnLoadCycle_m;
        bool hasTitle_m;

        void changeMenuPos(int8_t by){
            int sanity = 250;
            int16_t tmp = selectedOption_m;
            do {
                sanity--;
                tmp = (tmp + by);
                if (tmp < 0) tmp = numMenuOptions_m -1;
                if (tmp >= numMenuOptions_m) tmp = 0;
            } while (sanity >= 1 && attr_ma[tmp]==NOOP);

            startingIndex_m = max(0, tmp - MAX_ON_DISPLAY/2);
            selectedOption_m = tmp;
        }

};


