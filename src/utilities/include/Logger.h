
#pragma once
#include "RepeatingEvent.h"

class LogEntry{
    public:
        LogEntry(uint16 to):e(to){}
        template<typename... Args> 
        void log(const char * f, Args... args) {
            if (Serial.availableForWrite() && e.isReady()){
                Serial.printf(args...);
            }
        }
    private:
        RepeatingEvent e;
        
};