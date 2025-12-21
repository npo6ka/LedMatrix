#pragma once

#include "effect_list/effect.h"

class TvAllColor : public Effect
{
    int tick = 0;

public:
    void on_init()
    {
        set_fps(60);
    }

    void on_update()
    {
        uint16_t i;
        tick = tick % (256);
        tick += 1;

        for (auto x : LedMatrix.rangeX()) {
            for (auto y : LedMatrix.rangeY()) {
                uint8_t n = (tick + i) % (256);
                LedMatrix.at(x, y) = CHSV(n, 255, 255);
            }
        }
    }
};