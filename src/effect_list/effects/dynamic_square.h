#pragma once

#include "effect_list/effect.h"

class DynamicSquare : public Effect
{
    uint8_t fade_step = 192;
    uint8_t radius = std::min(LedMatrix.width(), LedMatrix.height());
    uint8_t hsv = 0;
    uint8_t cur_ring = 0;

public:
    DynamicSquare() {}

    void on_init() {
        set_fps(12);
    }

    void on_update() {
        LedMatrix.fader(fade_step);

        auto color = CHSV(hsv, 255, 255);
        auto offset = abs(-int(radius) / 2 + cur_ring);

        LedMatrix.draw_border(offset, offset, LedMatrix.width() - offset, LedMatrix.height() - offset, 1, color);

        cur_ring = (cur_ring + 1) % (radius);
        hsv = (hsv + 1) % 256;
    }
};
