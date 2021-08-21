#pragma once
#include "Alert.hpp"
class MCAlert: public Alert {
    public: 
        MCAlert():lastMinute(0),alarmName("Foo"){
        };
        
        
    private:
        char* alarmName;
        uint8 lastMinute;
        virtual void onLoad(){
            display_mp->clearDisplay();
            display_mp->setTextSize(2);
            display_mp->println("Changed!");
            display_mp->display();
            Alert::onLoad();
        };

        virtual void onHeartbeat(){
            if (displayManager_mp->isActiveScreen(this)) return;
            
            if (hasMinuteChanged()){
                displayManager_mp->addEnd(this);
            }
        }

        bool hasMinuteChanged(){
            time_t now = time(nullptr);
            auto t = localtime(&now);
            if (t->tm_min != lastMinute){
                lastMinute = t->tm_min;
                return true;
            }
            return false;
        }
};
