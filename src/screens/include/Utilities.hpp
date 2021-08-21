#pragma once
#include "Menu.hpp"
#include "Freezer.hpp"
class Utilities: public Menu {
    public: 
        Utilities():Menu("Utilities"){
            addSubDisplay("Freezer",&freezer);
            addBackAndExit();
            resetPosOnLoadCycle(true);
        };
        virtual ~Utilities(){}
    private:
        Freezer freezer;
};