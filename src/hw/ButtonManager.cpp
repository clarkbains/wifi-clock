#include "include/ButtonManager.h"
ButtonManager::ButtonManager():initializing(true), gotChange(true),readUpdate(true), stateChangeDelay_m(150), prevStates(0),
        currentState(0),lastStates(0), blockEvents(false){
        Button* upper = new Button(push, 0);
        Button* lower = new Button (push, 2);
        buttons[0] = lower;
        buttons[1] = upper;
        update();

};


void ButtonManager::update(){
    bool changed = false;
    for (int i =0; i<NUMBER_BTNS; i++){
        buttons[i]->update();
        if (buttons[i]->hasNewData()){
            changed = true;
            break;
        }
    }

    if (!changed){
        if (stateChangeDelay_m.shouldRun() && !gotChange){
            prevStates = currentState;
            currentState = lastStates;
            gotChange = true;

            if (initializing || blockEvents){
                readUpdate = true;
            } else {
                readUpdate = false;
            }
            if (currentState == 0)
                blockEvents = false;
            initializing = false;
        }
        return;
    };
    gotChange = false;
    lastStates      = states();
    stateChangeDelay_m.start();

}

uint8 ButtonManager::states(){
        uint8_t r = 0;
        for (int i =0; i<NUMBER_BTNS; i++)
            r+=((buttons[i]->getCurrentState()) << i);
        return r;
    }