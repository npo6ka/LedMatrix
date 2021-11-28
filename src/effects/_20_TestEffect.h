#pragma once

#include "effect.h"

class TestEffect : public Effect
{

private:
    uint32_t time;
    uint32_t tick;
    
    uint8_t step = 2;
    
public:
    TestEffect() {}

    void on_init() {
        tick = 0;
        time = 500;
        FastLED.clear();
        set_fps(60);
    }

    // void setColorAll(CRGB c){
    //     for (int i = 0; i < HEIGHT;  ++i) {
    //         for (int j = 0; j < WIDTH;  ++j) {
    //             getPix(i, j) = c;
    //         }   
    //     }
    // }

    void on_update() {
        // tick++;
        // if (tick < time){
        //     setColorAll(CRGB(255, 255, 255));
        //     return;
        // }
        // if (tick < time * 2){
        //     setColorAll(CRGB(0, 0, 255));
        //     return;
        // }
        // if (tick < time * 3){
        //     setColorAll(CRGB(0, 255, 0));
        //     return;
        // }
        
        // if (tick < time * 4){
        //     setColorAll(CRGB(255, 0, 0));
        //     return;
        // }
        // tick = 0;

        int i;
   
        for (i = 0; i < LEDS_CNT; ++i) {
            CRGB &cl = getLeds()[i];
            uint16_t u = 1024;
            if (random16(u) == 0) {
                cl = CRGB(255, 128, 0);
            } else if (cl.r > 0) {
                if (cl.r > step) {
                    cl = CRGB(cl.r - step, cl.g, 0);
                } else {
                    cl = CRGB(0, 0, 0);
                }

                if (cl.g > step) {
                    int k = step / 2 + step % 2;
                    cl = CRGB(cl.r, cl.g - k, 0);
                } else {
                    cl = CRGB(0, 0, 0);
                }
            }
        }
    }

    void updatePixel(int w, int h, int color){
        getPix(w, h).setColorCode(color);
    }

};
