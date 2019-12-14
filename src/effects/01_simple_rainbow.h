#pragma once

#include "effect.h"

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

        set_fps(30);
    }

    void on_update()
    {
        uint8_t x, y;
        tick = tick % (MAX_HSV + 1);

        for (x = 0; x < HEIGHT; x++) {
            for (y = 0; y < WIDTH; y++) {
                getPix(x, y) = CHSV((tick + x + y * phaseShift / 2) % (MAX_HSV + 1), 255, 255);
            }
        }

        tick += 1;
    }
};