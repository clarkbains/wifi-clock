#pragma once
#include "time.h"
#include "../../utilities/include/RepeatingEvent.h"
class TimeManager {
    public:
        static void update(){
            if (re.isReady() || t == NULL){
                time_t now = time(nullptr);
                t = localtime(&now);
            }
        }
        
        static struct tm* now(){
            return t;
        }

        static bool isReady(){
            update();
            return time(nullptr) > 48*3600;
        }
    private:
        static struct tm* t;
        static RepeatingEvent re;
};