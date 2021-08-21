#include "include/Menu.hpp"

Menu::Menu(char* title):numMenuOptions_m(0),startingIndex_m(0), 
    selectedOption_m(NO_SELECTION_YET),resetMenuPosOnLoadCycle_m(false),
    hasTitle_m(false){

    if (title != NULL){
        hasTitle_m = true;
        char* titleSpace = (char*) malloc(strlen(title) + 2);
        snprintf(titleSpace, strlen(title) + 2, " %s", title);
        addSpecialEntry(titleSpace, NOOP);
    }
};
void Menu::onButton(uint8_t now, uint8_t old){
    if ((now^old)&now == 0)
        return;
    if (now == 0b11){

        switch (attr_ma[selectedOption_m])
        {
        case EXIT:  displayManager_mp->goToPrimary();  break;
        case BACK:  displayManager_mp->popStack();     break;
        default: 
            if (funcs_ma[selectedOption_m]!=NULL){
                funcs_ma[selectedOption_m]->run(text_map[selectedOption_m]);
            }
            if (displays_map[selectedOption_m]!=NULL){
                displayManager_mp->addEnd(displays_map[selectedOption_m]);
            }
        }
    } else if (now == 0b10){
        changeMenuPos(-1);
    } else if (now == 0b01){
        changeMenuPos(1);
    }
};

void Menu::update(){
    onUpdate();
    display_mp->clearDisplay();
    display_mp->setCursor(0,0);
    display_mp->setTextSize(2);
    
    for (uint8_t i = startingIndex_m; 
        i < min(numMenuOptions_m, (uint8_t) (startingIndex_m + MAX_ON_DISPLAY)); 
        i++){
        if (i == selectedOption_m){
            uint8_t heightPerRow = display_mp->height()/MAX_ON_DISPLAY;
            display_mp->writeFillRect(0,(i-startingIndex_m) * heightPerRow,
            display_mp->width(), heightPerRow, WHITE);
        }
        display_mp->setTextColor(i == selectedOption_m? BLACK:WHITE);
        display_mp->println(text_map[i]);
    }
    display_mp->display();
};