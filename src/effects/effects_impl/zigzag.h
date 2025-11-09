#pragma once

#include "effects/effect.h"

class ZigZag : public Effect
{
    uint8_t x;
    uint8_t y;
    bool dir;
    uint8_t hue;
    uint8_t step;
    uint8_t tick;

public:
    ZigZag(const std::string& variableStoragePath) :
        Effect(variableStoragePath) {
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

        if (dir) {
            if (x >= LedMatrix.width()) {
                y++;
                dir = !dir;
            } else {
                x++;
            }
        } else {
            if (x - 1 < 0) {
                y++;
                dir = !dir;
            } else {
                x--;
            }
        }

        if (y >= LedMatrix.height()) {
            y = 0;
        }
    }
};
