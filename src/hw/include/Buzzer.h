
#pragma once
#include <Arduino.h>
#include "../../utilities/include/RepeatingEvent.h"
enum BuzzerMode {RANDOM, CONSTANT, OFF};
class Buzzer
{
public:
    Buzzer(uint8 p = 13, uint16 fChangeF = 50):fChange(fChangeF), pin(p){
        randomSeed(analogRead(0));
        bm=OFF;
    }
    void start(uint16 frequency){
        tone(pin, (double)frequency);
        bm = CONSTANT;
        Serial.printf("Started Buzzer at %d hz\n", frequency);
    }
    void start(){
        Serial.printf("Started Buzzer\n");
        tone(pin, (double)genFrequency());
        bm = RANDOM;
        fChange.markFinished();
    }
    void update(){
        if(fChange.isReady() && bm == RANDOM){
            tone(pin, (double)genFrequency());
        }
    }
    void stop(){
        noTone(pin);
        bm = OFF;
    }
    long genFrequency(){
        return random(300L, 1100L);    
    }

private:
    RepeatingEvent fChange;
    BuzzerMode bm;
    uint8 pin;
};