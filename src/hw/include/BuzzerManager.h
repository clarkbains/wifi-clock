
#pragma once
#include "Buzzer.h"
class BuzzerManager
{
public:
   static Buzzer& instance(){
       return b;
   }

private:
   static Buzzer b;
};