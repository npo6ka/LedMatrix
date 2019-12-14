#pragma once

#include "effect.h"

class AllRandom : public Effect
{

public:
    AllRandom() {}

    void on_init() {
        set_fps(30);
    }

    void on_update() {
        int i;

        for (i = 0; i < LEDS_CNT; i++) {
            getPix(i / WIDTH, i % WIDTH) = CRGB(random8(255), random8(255), random8(255));
        }
    }
};
