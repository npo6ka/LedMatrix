#pragma once

#include "effect_list/effect.h"

class Dribs : public Effect
{
    uint8_t cur_drib[WIDTH];
    uint8_t lenght;
public:
    void on_init()
    {
        lenght = 12;
        set_fps(30);
    }

    void on_update()
    {
        for (int j = 0; j < WIDTH;  ++j) {
            if (cur_drib[j] == 0 && random8(40) == 0) {
                cur_drib[j] = 1;
            } else if (cur_drib[j] != 0) {
                cur_drib[j] += 1;
                if (cur_drib[j] >= lenght + WIDTH) {
                    cur_drib[j] = 0;
                }
            }
        }

        int step = 255 / lenght;
        for (int i = 0; i < HEIGHT;  ++i) {
            for (int j = 0; j < WIDTH;  ++j) {
                if (cur_drib[j] != 0 && i < cur_drib[j]) {
                    getPix(i, j) = CRGB(0, 0, max(255 - (cur_drib[j] - i - 1) * step, 0));
                } else {
                    getPix(i, j) = 0x0;
                }
            }
        }
    }
};