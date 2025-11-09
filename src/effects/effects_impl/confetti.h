#pragma once

#include "effects/effect.h"

class Confetti : public Effect
{
    uint8_t brightness_step = 30;
    uint8_t density = 3;
public:
    Confetti(const std::string& variableStoragePath) :
        Effect(variableStoragePath) {}

    void on_init() {
        set_fps(30);
    }

    void on_update() {
        for (uint8_t i = 0; i < density; i++) {
            index_t x = random(LedMatrix.width());
            index_t y = random(LedMatrix.height());
            auto& pix = LedMatrix.at(x, y);
            if (!pix) {
                pix = CHSV(random8(255), 255, 255);
            }

            LedMatrix.fader(brightness_step);
        }
    }
};
