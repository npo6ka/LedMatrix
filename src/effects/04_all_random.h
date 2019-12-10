#pragma once

#include "effect.h"

class AllRandom : public Effect
{
    uint8_t step = 2;

public:
    AllRandom() {}

    void on_init() {

    }

    void on_tick() {
        int i;

        for (i = 0; i < LEDS_CNT; i++) {
            getPix(i / 10, i % 10) = CRGB(random8(255), random8(255), random8(255));
        }
    }
};
