#pragma once

#include "effect.h"

class TestEffect : public Effect
{
    
public:
    TestEffect() {}

    void on_init() {
        FastLED.clear();
    }

    void on_update() {

    }

    void updatePixel(int w, int h, int color){
        getPix(w, h).setColorCode(color);
    }

};
