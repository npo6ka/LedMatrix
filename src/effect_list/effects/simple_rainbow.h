#pragma once

#include "effect_list/effect.h"

class SimpleRainbow : public Effect
{
    int tick;
    int phaseShift;
public:
    SimpleRainbow() {}

    void on_init()
    {
        tick = 0;
        phaseShift = 10;

        set_fps(10);
    }

    void on_update()
    {
        uint16_t x, y;
        tick = tick % (256);

        for (x = 0; x < HEIGHT; x++) {
            for (y = 0; y < WIDTH; y++) {
                getPix(x, y) = CHSV((tick + x + y * phaseShift / 2) % (256), 255, 255);
            }
        }

        tick += 1;
    }
};