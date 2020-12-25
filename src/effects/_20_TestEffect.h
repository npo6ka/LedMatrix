#pragma once

#include "effect.h"

class TestEffect : public Effect
{

private:
    uint32_t time;
    uint32_t tick;
    
public:
    TestEffect() {}

    void on_init() {
        tick = 0;
        time = 500;
        FastLED.clear();

       setColorAll(CRGB(255, 255, 255));
    }

    void setColorAll(CRGB c){
        for (int i = 0; i < HEIGHT;  ++i) {
            for (int j = 0; j < WIDTH;  ++j) {
                getPix(i, j) = c;
            }   
        }
    }

    void on_update() {
        tick++;
        if (tick < time){
            setColorAll(CRGB(255, 255, 255));
            return;
        }
        if (tick < time * 2){
            setColorAll(CRGB(0, 0, 255));
            return;
        }
        if (tick < time * 3){
            setColorAll(CRGB(0, 255, 0));
            return;
        }
        
        if (tick < time * 4){
            setColorAll(CRGB(255, 0, 0));
            return;
        }
        tick = 0;
    }

    void updatePixel(int w, int h, int color){
        getPix(w, h).setColorCode(color);
    }

};
