#pragma once

#include "effects/effect.h"

class ZigZag : public Effect
{
    index_t x;
    index_t y;
    bool dir;
    uint8_t hue;
    uint8_t step;
    uint8_t tick;

public:
    ZigZag() {
        set_fps(20);
    }

    void on_init() {
        x = 0;
        y = 0;
        step = 2;
        dir = true;
        hue = random8();
        tick = 0;
    }

    void on_update(void) {
        LedMatrix.fader(1);
        LedMatrix.at(x, y) = CHSV(hue, 255, 255);

        tick++;
        if (tick >= step) {
            tick = 0;
            hue++;
        }

        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();

        if (w <= 1) {
            y = (h == 0) ? 0 : (y + 1) % h;
            return;
        }

        if (dir) {
            if (x + 1 >= w) {
                y = (h == 0) ? 0 : (y + 1) % h;
                dir = false;
            } else {
                x++;
            }
        } else {
            if (x == 0) {
                y = (h == 0) ? 0 : (y + 1) % h;
                dir = true;
            } else {
                x--;
            }
        }
    }
};
