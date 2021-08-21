#pragma once
#include <Adafruit_GFX.h>
#include "../../hw/include/SSD1306Brightness.h"
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include "../../hw/include/ButtonManager.h"
class DisplayManager;

class Screen
{
friend DisplayManager;

public:
    Screen():display_mp(NULL),displayManager_mp(NULL){}
    void setDisplay(SSD1306Brightness* s){
        display_mp = s;
    }
    void setDM(DisplayManager* d){
        displayManager_mp = d;
    }
private:
    //For Active Displays. Called once every ~10ms
   virtual void update(){};
   virtual void onHeartbeat(){};
   virtual void onLoad(){};
   virtual void onUnload(){};
   virtual void onButton(uint8_t now, uint8_t prev){onButton(now);};
   virtual void onButton(uint8_t now){ };

   virtual uint8 getPriority()
   { return 100; };

protected:
    SSD1306Brightness* display_mp;
    DisplayManager*   displayManager_mp;
};

class ConnectingDisplay: public Screen{
    public:
        ConnectingDisplay(const char *c):connectingItem_mp(c){};



    private:
        const char* connectingItem_mp;

        virtual void onLoad(){
        display_mp->clearDisplay();
        display_mp->setTextSize (2);
        display_mp->setCursor(0, 0);
        display_mp->println(connectingItem_mp);
        display_mp->print ("Connecting");
        display_mp->display();
    }
};

