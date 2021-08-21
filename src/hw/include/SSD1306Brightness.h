#pragma once
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
class SSD1306Brightness: public Adafruit_SSD1306{
    using Adafruit_SSD1306::Adafruit_SSD1306;
    public:
        const static uint8 NUM_BRIGHTNESS_STEPS = 5;

        void initBrightnessControl(){
            currentStep_m = 0;
            setBrightness();
        }

        uint8 getCurrentBrightness() {
            return currentStep_m;
        }

        void brighter(){
            setBrightness(currentStep_m+1);
        }

        void dimmer(){
            setBrightness(currentStep_m-1);   
        }

        void setBrightness(int8 b){
            currentStep_m = min(max((int8)0, b),(int8)(NUM_BRIGHTNESS_STEPS-1));
            setBrightness();
        }



    private:
        int8 currentStep_m;
        const byte DBL[NUM_BRIGHTNESS_STEPS][3] =
            {{0, 0, 0},
            {5, 2, 0},
            {40, 18, 0},
            {60, 26, 32},
            {150, 34, 64}};
        void setBrightness () {
            ssd1306_command(0xD9); //0xD9
            ssd1306_command(DBL[currentStep_m][1]); //0xF1 default, to lower the contrast, put 1-1F
            ssd1306_command(0x81);
            ssd1306_command(DBL[currentStep_m][0]); // 0-255
            /*ssd1306_command(0xDB); //, (additionally needed to lower the contrast)
            ssd1306_command(DBL[currentStep_m][2]); //0x40 default, to lower the contrast, put 0
            ssd1306_command(0xA4);
            ssd1306_command(0xA6);
            ssd1306_command(0xAF);*/
        }

};