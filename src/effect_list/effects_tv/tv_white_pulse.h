#pragma once

#include "effect_list/effect.h"

class TvWhitePulse : public Effect
{
    uint8_t light_white = 0;
    uint8_t light_white_step = 2;

public:
    void on_init()
    {
        set_fps(60);
    }

    void on_update()
    {
        light_white += light_white_step;
        if (light_white >=250){
            light_white_step = -2;
            light_white = 250;
        }

        if (light_white <=50){
            light_white_step = 2;
            light_white = 50;
        }

        CRGB pixel =CRGB(light_white, light_white, light_white);

        for (auto x : LedMatrix.rangeX()) {
            for (auto y : LedMatrix.rangeY()) {
                LedMatrix.at(x, y) = pixel;
            }
        }
    }
};