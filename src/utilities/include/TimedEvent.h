
#pragma once
#include <Arduino.h>

class TimedEvent{
    public:
    void setTriggerOnNextInvocation(bool = true);

    protected:
        TimedEvent(uint16 d);
        bool hasTimerPassed();
        void resetTimer();
        void setPeriod(uint16);
    private:
        static const uint32 MAX_PERIOD = UINT16_MAX;
        uint32 nextRunInMS_m;
        uint32 period;
        bool triggerOnNextInvocation_m;

};
