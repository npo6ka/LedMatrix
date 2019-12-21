#pragma once

#include "effect.h"

class RainbowRain : public Effect
{
    uint8_t fade_step;
    uint8_t rainbow_step;
    int tick;

public:
    RainbowRain() {}

    void on_init()
    {
        fade_step = 3;
        rainbow_step = 4;
        tick = 0;
        set_fps(40);
    }

    void on_update()
    {
        int i;

        for (i = 0; i < LEDS_CNT; ++i) {
            CRGB &cl = getLeds()[i];
            cl.fadeToBlackBy(fade_step);

            if (random16(300) == 0) {
                cl = CHSV(tick / rainbow_step, 255, 255);
            }
        }

        tick = (tick + 1) % (256 * rainbow_step);
    }
};