#pragma once

#include "effects/effect.h"

class Dribs : public Effect
{
    index_t cur_drib[LEDS_WIDTH];
    uint8_t lenght;
public:
    void on_init()
    {
        lenght = 12;
        memset(cur_drib, 0, sizeof(cur_drib));
        set_fps(30);
    }

    void on_update()
    {
        const index_t h = LedMatrix.height();
        const index_t limit = lenght + h;

        for (auto j : LedMatrix.rangeX()) {
            if (cur_drib[j] == 0 && random8(40) == 0) {
                cur_drib[j] = 1;
            } else if (cur_drib[j] != 0) {
                cur_drib[j] += 1;
                if (cur_drib[j] >= limit) {
                    cur_drib[j] = 0;
                }
            }
        }

        int step = 255 / lenght;
        for (auto x : LedMatrix.rangeX()) {
            for (auto y : LedMatrix.rangeY()) {
                if (cur_drib[x] != 0 && y < cur_drib[x]) {
                    LedMatrix.at(x, y) = CRGB(0, 0, max(255 - (int)(cur_drib[x] - y - 1) * step, 0));
                } else {
                    LedMatrix.at(x, y) = 0x0;
                }
            }
        }
    }
};
