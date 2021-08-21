
#pragma once
#include "TimedEvent.h"

class RepeatingEvent: public TimedEvent{
    public:
        RepeatingEvent(uint16 = 1000, bool = true);
        bool isReady();
        void markFinished();
        void setCooldown(uint16);
        void setAutoReset(bool);
    private:
        bool autoreset_m;
};