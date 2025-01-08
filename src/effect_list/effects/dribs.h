#pragma once

#include "effect_list/effect.h"

class Dribs : public Effect
{
    uint8_t cur_drib[LEDS_WIDTH];
    uint8_t lenght;
public:
    void on_init()
    {
        lenght = 12;
        set_fps(30);
    }

    void on_update()
    {
        for (auto j : LedMatrix.rangeX()) {
            if (cur_drib[j] == 0 && random8(40) == 0) {
                cur_drib[j] = 1;
            } else if (cur_drib[j] != 0) {
                cur_drib[j] += 1;
                if (cur_drib[j] >= lenght + LedMatrix.width()) {
                    cur_drib[j] = 0;
                }
            }
        }

        int step = 255 / lenght;
        for (auto x : LedMatrix.rangeX()) {
            for (auto y : LedMatrix.rangeY()) {
                if (cur_drib[x] != 0 && y < cur_drib[x]) {
                    LedMatrix.at(x, y) = CRGB(0, 0, max(255 - (cur_drib[x] - y - 1) * step, 0));
                } else {
                    LedMatrix.at(x, y) = 0x0;
                }
            }
        }
    }
};