#pragma once

#include "effects/effect.h"

class RainbowRain : public Effect
{
    uint8_t fade_step;
    uint8_t rainbow_step;
    int tick;

public:
    RainbowRain(const std::string& variableStoragePath) :
        Effect(variableStoragePath) {}

    void on_init()
    {
        fade_step = 3;
        rainbow_step = 4;
        tick = 0;
        set_fps(40);
    }

    void on_update()
    {
        for (size_t i = 0; i < LedMatrix.size(); ++i) {
            auto& led = LedMatrix.at(i);
            led.fadeToBlackBy(fade_step);

            if (random16(300) == 0) {
                led = CHSV(tick / rainbow_step, 255, 255);
            }
        }

        tick = (tick + 1) % (256 * rainbow_step);
    }
};