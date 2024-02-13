#pragma once

#include "effect_list/effect.h"

class RandomRain : public Effect
{
    uint8_t step;

public:
    RandomRain() {}

    void on_init()
    {
        step = 2;
        set_fps(30);
    }

    void on_update()
    {
        for (uint16_t i = 0; i < LEDS_CNT; ++i) {
            CRGB &cl = getLeds()[i];
            cl.fadeToBlackBy(step);

            if (random16(500) == 0) {
                cl.r = 255;
            }
            if (random16(500) == 0) {
                cl.g = 255;
            }
            if (random16(500) == 0) {
                cl.b = 255;
            }
        }
    }
};