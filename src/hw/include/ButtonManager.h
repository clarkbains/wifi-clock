#pragma once
#include "Button.h"
#include "../../utilities/include/DelayedEvent.h"

class ButtonManager
{
public:
    ButtonManager();

    void    block()             { blockEvents = true; }
    bool    hasNewData()        { return !readUpdate;  }
    uint8 normalizedStates()  { return currentState;}
    uint8 normalizedPrevStates()  { return prevStates;}
    void    clearReadFlag()     { readUpdate = true; }

    void update();

private:
    static const uint8 NUMBER_BTNS = 2;
    Button* buttons[NUMBER_BTNS];
    DelayedEvent  stateChangeDelay_m;
    
    uint8 currentState;
    uint8 lastStates;
    uint8 prevStates;
    bool    readUpdate;
    bool    initializing;
    bool    gotChange;
    bool    blockEvents;
    uint8 states();
};
