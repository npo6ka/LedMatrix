#pragma once

#include "effect.h"

class Empty : public Effect
{
    
public:
    Empty() {}

    void on_init() {
        FastLED.clear();
    }

    void on_update() {

    }

    void updatePixel(int w, int h, int color){
        getPix(w, h).setColorCode(color);
    }

};
