#pragma once

#include "effect.h"

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
        int i, j;

        for (i = 0; i < WIDTH; ++i) {
            for (j = 0; j < HEIGHT; ++j) {
                fadePix(i, j, step);

                CRGB cl = getPix(i, j);

                if (random16(500) == 0) {
                    cl.r = MAX_BRIGHTNESS;
                }
                if (random16(500) == 0) {
                    cl.g = MAX_BRIGHTNESS;
                }
                if (random16(500) == 0) {
                    cl.b = MAX_BRIGHTNESS;
                }

                setPixColor(i, j, cl);
            }
        }
    }
};