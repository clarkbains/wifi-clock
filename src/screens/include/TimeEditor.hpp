#pragma once
#include "Menu.hpp"
#include "../../lib/include/AlarmManager.hpp"
class HrEditor;
class MinEditor;


class HrEditor: public Menu {
    public: 
        HrEditor(WakeAlarm* w):Menu( (char*) w->getName()), wa(w){
            addSpecialEntry(w->getTOD(), NOOP);
            MenuFunction* mf =  new MenuFunction(this,[](char* c, void* v){
                auto self = (HrEditor*) v;
                int8 d = c[1] - 48;
                if (c[0] == '-'){
                    d*=-1;
                }
                self->wa->changeHr(d);
                self->wa->getTOD();
            });
            addSubFunc("+1", mf);
            addSubFunc("-1", mf);
            
            addBackAndExit();
            resetPosOnLoadCycle(true);
        };
    private:
        WakeAlarm* wa;
    
};
class MinEditor: public Menu {
    public: 
        MinEditor(WakeAlarm* w):Menu( (char*) w->getName()), wa(w){
            addSpecialEntry(w->getTOD(), NOOP);
            MenuFunction* mf =  new MenuFunction(this,[](char* c, void* v){
                auto self = (MinEditor*) v;
                int8 d = c[1] - 48;
                if (c[0] == '-'){
                    d*=-1;
                }
                self->wa->changeMin(d);
                self->wa->getTOD();

            });
            addSubFunc("+5", mf);
            addSubFunc("-5", mf);
            addBackAndExit();
            resetPosOnLoadCycle(true);
        };
    private:
        WakeAlarm* wa;
};

class TimeEditor: public Menu {
    public: 
        TimeEditor(WakeAlarm* w):Menu( (char*) w->getName()), h(w), m(w), wa(w){
            addSpecialEntry(w->getTOD(), NOOP);
            addSubDisplay("edit hr", &h);
            addSubDisplay("edit min", &m);
            addBackAndExit();
            resetPosOnLoadCycle(true);
        };
    private:
        HrEditor h;
        MinEditor m;
        WakeAlarm* wa;
        virtual void onUpdate(){

            wa->getTOD();
        }

    
};