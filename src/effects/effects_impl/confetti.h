#pragma once

#include "effects/effect.h"

class Confetti : public Effect
{
    uint8_t brightness_step = 12;
    uint8_t density;
public:
    Confetti() {}

    void on_init() {
        size_t count = LedMatrix.size() / 8;
        if (count < 3) count = 3;
        if (count > 40) count = 40;
        density = (uint8_t)count;
        set_fps(30);
    }

    void on_update() {
        LedMatrix.fader(brightness_step);

        for (uint8_t i = 0; i < density; i++) {
            index_t x = random(LedMatrix.width());
            index_t y = random(LedMatrix.height());
            auto& pix = LedMatrix.at(x, y);
            if (!pix) {
                pix = CHSV(random8(255), 255, 255);
            }
        }
    }
};
