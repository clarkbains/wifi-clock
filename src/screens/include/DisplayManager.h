
/*
    Maintains a collection of displays, and periodically updates the active one

*/
#pragma once
#include "Screen.hpp"
#include "../../hw/include/ButtonManager.h"
#include "../../hw/include/SSD1306Brightness.h"
#include "../../utilities/include/RepeatingEvent.h"

struct heartbeat_t {
    Screen* d;
    RepeatingEvent re;
};

class DisplayManager
{
public:
    DisplayManager(SSD1306Brightness& s):display_mr(s),
    currentStackSize_m(0),currentHeartbeat_m(0){
    }

    void init(Screen* d){
        display_mr.clearDisplay();
        display_mr.setTextColor(WHITE);
        display_mr.display();
        addEnd(d);
    }

    //Removes 
    void killTop(uint8_t levels){
        bool removedScreens = false;
        for (uint8_t i = 0; i<levels; i++){
            if (currentStackSize_m == 1)
                break;
            deleteEnd();
            removedScreens = true;
        }
        if (removedScreens) stackMapEnd()->onLoad();
    }

    void goToPrimary(){
        killTop(currentStackSize_m - 1);
    }

    uint8_t getStackPos(){ return currentStackSize_m; };

    void addEnd(Screen *d){
        if (currentStackSize_m == STACK_SIZE_s){
            return;
        }
        stack_map[currentStackSize_m++] = d;
        d->setDM(this);
        d->setDisplay(&display_mr);

        load(d);
    }

    void addHeartBeatDisplay(Screen *d, uint16 period = 1000){
        if (currentHeartbeat_m == HEARTBEAT_SIZE_s){
            return;
        }
        d->setDM(this);
        d->setDisplay(&display_mr);

        heartbeat_ma[currentHeartbeat_m].d = d;
        heartbeat_ma[currentHeartbeat_m].re.setAutoReset(false);
        heartbeat_ma[currentHeartbeat_m++].re.setCooldown(period);
    }

    Screen* stackMapEnd(){
        return stack_map[currentStackSize_m-1];
    }

    void replace(Screen *d){
        deleteEnd();
        addEnd(d);
    }

    void popStack(){
        deleteEnd();
        load(stackMapEnd());
    }

    void update(){     
        btnMgr_m.update();
        if (btnMgr_m.hasNewData()){
            stackMapEnd()->onButton(btnMgr_m.normalizedStates(), btnMgr_m.normalizedPrevStates());
            btnMgr_m.clearReadFlag();
        }
        stack_map[currentStackSize_m-1]->update();
        
        for (uint8_t i = 0; i<currentHeartbeat_m; i++){
            if (heartbeat_ma[i].re.isReady()){
                if (stackMapEnd()->getPriority() 
                        >= heartbeat_ma[i].d->getPriority()){
                    heartbeat_ma[i].d->onHeartbeat();
                    heartbeat_ma[i].re.markFinished();
                }
            }
        }
    }
    bool isActiveScreen(Screen* d){
        return stack_map[currentStackSize_m-1] == d;
    }

    void load(Screen* d){
        display_mr.setTextColor(WHITE);
        display_mr.setCursor(0,0);
        display_mr.setFont();
        
        d->onLoad();

        //Will prevent the release event from sending to the next
            //display if pressing the button caused the switch
        if (btnMgr_m.normalizedStates() != 0){
            btnMgr_m.block();
        }
    }

private:
    void deleteEnd(){
        if (currentStackSize_m < 1)
            return;
        currentStackSize_m--;    
        stack_map[currentStackSize_m]->onUnload();
    }
    static const uint8_t STACK_SIZE_s = 10;
    Screen* stack_map[STACK_SIZE_s];
    uint8_t  currentStackSize_m;

    static const uint8_t HEARTBEAT_SIZE_s = 10;
    heartbeat_t heartbeat_ma[HEARTBEAT_SIZE_s];
    uint8_t  currentHeartbeat_m;
    
    SSD1306Brightness& display_mr;
    ButtonManager    btnMgr_m;     

};


