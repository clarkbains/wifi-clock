#pragma once
#include "Screen.hpp"
#include "../../utilities/include/RepeatingEvent.h"
#include <ESP8266HTTPClient.h>
#include "../../../definitions.h"
#include "DisplayManager.h"
#include "../../utilities/include/Logger.h"

#define NO_TEMP 999.99F

class Freezer: public Screen { 
    public:
        Freezer():poll(1500){

        };  
    private:
        float getTemp(){
            http.begin(freezerUrl.c_str());
            int code = http.GET();
            float retVal;
            if (code == 200){
                retVal = http.getString().toFloat();
            } else {
                retVal = NO_TEMP;
            }
            Serial.printf("Freezer Request. %d. %s. %f\n", code, http.getString().c_str(), retVal);
            http.end();

            return retVal;
        }
        virtual void update(){
            if (poll.isReady()){
                display();
            }
        }

        virtual void display(){
            float f = getTemp();
            display_mp->clearDisplay();
            display_mp->setCursor(0,31);
            if (f!=NO_TEMP){
                display_mp->setTextSize(1);
                display_mp->setFont (&FreeSans18pt7b);
                display_mp->printf("%.2f\n", f);
                display_mp->setFont();
                display_mp->setCursor(15,40);
                display_mp->setTextSize(2);
                display_mp->println("real-time");
            } else {
                display_mp->setTextSize(4);
                display_mp->printf("Unable");
                display_mp->setTextSize(2);
                display_mp->println("to connect");
                display_mp->println("to server");
            }
            display_mp->display();
        }
        virtual void onLoad(){
            display_mp->clearDisplay();
            display_mp->setTextColor(WHITE);
            display_mp->display();
            display();
        };

        virtual void onButton(uint8_t now){
            displayManager_mp->popStack();
        };
    RepeatingEvent poll;
    HTTPClient http;

};