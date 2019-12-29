#pragma once

#include "effect.h"

#define START_LENGTH 4    // начальная длина змейки
#define MAX_LENGTH 80     // максимальная длина змейки

class Snake : public Effect
{
    uint8_t inc_val[LEDS_CNT];
    int step;

    int n1;
    int n2;
    int n3;

public:
    Snake() {}

    void on_init() {
        step = 2;
        set_fps(10);
        ini_n1();
    }

    void ini_n1(){
        n1= 1;
    }

    void ini_n2(){
        n2= 2;
    }

    void ini_n3(){
        n3= 3;
    }

    void on_update() {
        myUpdate();


    }

    void myUpdate(){
        if (n1 + 1 > HEIGHT){
            ini_n1();
        }
        if (n2 + 1 > HEIGHT){
            ini_n2();
        }
        if (n3 + 1 > HEIGHT){
            ini_n3();
        }
        
        uint8_t x, y;

        // for (x = 0; x < HEIGHT; x++) {
        //     for (y = 0; y < WIDTH; y++) {
        //         getPix(x, y) = 0x000000;
        //     }
        // }
        FastLED.clear();

        getPix(n1, 0) = 0xFFFFFF;
        getPix(n2, 2) = 0xFF00FF;
        getPix(n3, 10) = 0x000F0F;

        n1++;
        n2++;
        n3++;
    }

};
