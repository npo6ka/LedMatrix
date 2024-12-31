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

        set_fps(30);
    }

    void on_update()
    {
        tick = tick % (256);

        for (auto x : LedMatrix.rangeX()) {
            for (auto y : LedMatrix.rangeY()) {
                LedMatrix.at(x, y) = CHSV((tick + x + y * phaseShift / 2) % (256), 255, 255);
            }
        }

        tick += 1;
    }
};