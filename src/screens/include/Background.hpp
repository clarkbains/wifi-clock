#pragma once
#include "DisplayManager.h"
#include "../background/include/Alert.hpp"
#include "../background/include/Alarm.hpp"
class Background: public Alert {
    public: 

        Background():Alert(800){
        };

        virtual void onLoad(){
            display_mp->clearDisplay();
            display_mp->setTextSize(2);
            display_mp->println("Setting Up");
            display_mp->println("Background");
            display_mp->println("Tasks");
            display_mp->display();
            setupHeartbeats(); 
            Alert::onLoad();
        };


        void setupHeartbeats(){
            displayManager_mp->addHeartBeatDisplay(&ap,100);
        //    displayManager_mp->addHeartBeatDisplay(&ra, 2500);
        //    displayManager_mp->addHeartBeatDisplay(&mca, 5000);
        }

    private:
//        ReccurringAlarm ra;
 //       MCAlert mca;
            AlarmProcessor ap;
};
