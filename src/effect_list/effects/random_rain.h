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
        for (size_t i = 0; i < LedMatrix.size(); ++i) {
            auto& led = LedMatrix.at(i);
            led.fadeToBlackBy(step);

            if (random16(500) == 0) {
                led.r = 255;
            }
            if (random16(500) == 0) {
                led.g = 255;
            }
            if (random16(500) == 0) {
                led.b = 255;
            }
        }
    }
};