
#pragma once
#include <Arduino.h>
enum ButtonType {push, toggle};
class Button
{
private:
    boolean currentState;
    boolean lastFlickerState;
    boolean lastSteadyState;
    boolean state;
    ButtonType bt;
    boolean read;
    long pauseUntil;
    unsigned char debounceMs;
    unsigned char pin;
    bool readButton();
    

public:
    Button(ButtonType, unsigned char);
    void update();
    boolean hasNewData();
    boolean getCurrentState();
    void init();
    void clearReadFlag();

};