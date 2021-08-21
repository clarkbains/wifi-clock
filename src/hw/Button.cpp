#include "include/Button.h"

Button::Button(ButtonType bt, unsigned char p){
    this->pin = p;
    this->bt = bt;
    switch(this->bt){
        case push: this->debounceMs = 50; pinMode (this->pin, INPUT_PULLUP); break;
        case toggle: this->debounceMs = 50; break;
    }
    this->pauseUntil = 0;
}


void Button::init(){
    this->state = readButton();
    this->lastSteadyState = this->state;
    this->lastFlickerState = this->state;
    update();
    this->read = true;

}
boolean Button::getCurrentState(){
    this->read = true;
    return state;
}

boolean Button::hasNewData(){
    return !this->read;
}

bool Button::readButton(){
    if (this->bt == push){
        return !digitalRead(this->pin);
    } else if (this->bt == toggle){
        return analogRead(this->pin) < 512;
    }
    return false;
}
void Button::clearReadFlag(){
    this->read = false;
}

void Button::update(){
    if (millis() < pauseUntil){
        return;
  }
    currentState = readButton();
    this->lastSteadyState = this->lastFlickerState;
    this->lastFlickerState = currentState;
    if (this->lastSteadyState != this->lastFlickerState){
        pauseUntil = millis()+20;
    } else {
        if (currentState!=this->state){
            this->state = currentState;
            this->read = false;
        }
    }

    
}
