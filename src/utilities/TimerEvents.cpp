#include "include/TimedEvent.h"
#include "include/RepeatingEvent.h"
#include "include/DelayedEvent.h"

TimedEvent::TimedEvent(uint16 ms):period(ms),triggerOnNextInvocation_m(false),nextRunInMS_m(0){
    resetTimer();
};

boolean TimedEvent::hasTimerPassed(){
    if (triggerOnNextInvocation_m || 
        millis() >= nextRunInMS_m && 
        millis() - nextRunInMS_m <= MAX_PERIOD){
        
        triggerOnNextInvocation_m = false;
        return true;
    }

    return false;
}


void TimedEvent::resetTimer(){
    uint32 timeToOverflowInMS = UINT32_MAX - millis();
    if(timeToOverflowInMS <= period)
        nextRunInMS_m = period - timeToOverflowInMS;
    else
        nextRunInMS_m = millis() + period;
}

void TimedEvent::setPeriod(uint16 p){
    period = p;
    resetTimer();
}

void TimedEvent::setTriggerOnNextInvocation(bool b){
    triggerOnNextInvocation_m = b;
}

RepeatingEvent::RepeatingEvent(uint16 ms, bool a):TimedEvent(ms), autoreset_m(a){
    resetTimer();
    setTriggerOnNextInvocation(true);
}

bool RepeatingEvent::isReady(){
    bool b = hasTimerPassed();
    if (autoreset_m && b){
        resetTimer();
    }
    return b;
}

void RepeatingEvent::setCooldown(uint16 v){
    setPeriod(v);
}

void RepeatingEvent::markFinished(){
    resetTimer();
}

void RepeatingEvent::setAutoReset(bool b){
    autoreset_m = b;
}

DelayedEvent::DelayedEvent(uint16_t ms):TimedEvent(ms){
    resetTimer();
}

bool DelayedEvent::shouldRun(){
    return hasTimerPassed();
}

void DelayedEvent::start(){
    resetTimer();
}