

#pragma once
#include "TimedEvent.h"
class DelayedEvent: public TimedEvent{
    public:
        DelayedEvent(uint16_t);
        bool shouldRun();
        void start();
};