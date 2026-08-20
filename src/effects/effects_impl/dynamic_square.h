#pragma once

#include "effects/effect.h"

class DynamicSquare : public Effect
{
    uint8_t fade_step = 192;
    uint8_t hsv = 0;
    index_t cur_ring = 0;

public:
    DynamicSquare() {}

    void on_init() {
        set_fps(12);
        cur_ring = 0;
    }

    void on_update() {
        LedMatrix.fader(fade_step);

        auto color = CHSV(hsv, 255, 255);
        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();
        const index_t radius = std::min(w, h);

        if (radius <= 1) {
            const index_t len = std::max(w, h);
            if (len == 0) return;
            const index_t pos = cur_ring % len;
            if (w <= 1) {
                LedMatrix.at(0, pos) = color;
            } else {
                LedMatrix.at(pos, 0) = color;
            }
            cur_ring = (cur_ring + 1) % len;
        } else {
            auto offset = abs(-int(radius) / 2 + (int)cur_ring);
            LedMatrix.drawRectBorder(offset, offset, w - offset, h - offset, 1, color);
            cur_ring = (cur_ring + 1) % radius;
        }

        hsv = (hsv + 1) % 256;
    }
};
