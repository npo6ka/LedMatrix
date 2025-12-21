#pragma once

#include "effect_list/effect.h"

class TvYellow : public Effect
{
    uint8_t lenght;

public:
    void on_init()
    {
        lenght = 0;
        set_fps(1);
    }

    void on_update()
    {
        if (lenght <= 1){
            for (auto x : LedMatrix.rangeX()) {
                for (auto y : LedMatrix.rangeY()) {
                    LedMatrix.at(x, y) = CRGB(255, 255, 10);
                }
            }
            lenght = 1;
        }
    }
};