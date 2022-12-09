#pragma once

#define ACCURACY 10

#include "effect_list/effect.h"

class RainbowStaticPoint : public Effect
{
    int tick;
    uint8_t tick_size = 4; //кол-во тиков до инкремента тика радуги

    void rainbow_static_point_render_point() {
        int i, j;
        int x = HEIGHT / 2 * ACCURACY;
        int y = WIDTH / 2 * ACCURACY;

        for (i = 0; i < HEIGHT; ++i) {
            for(j = 0; j < WIDTH; ++j) {
                int loc_x = i * ACCURACY + ACCURACY / 2;
                int loc_y = j * ACCURACY + ACCURACY / 2;

                int distance = sqrt((loc_x - x) * (loc_x - x) + (loc_y - y) * (loc_y - y));

                float chsv = (distance + tick / tick_size) % 255;

                getPix(i, j) = CHSV(chsv, 255, 255);
            }
        }
    }

public:
    RainbowStaticPoint() {}

    void on_init()
    {
        tick = 0;
        set_fps(60);
    }

    void on_update()
    {
        tick = (tick + 1) % (256 * tick_size);
        rainbow_static_point_render_point();
    }
};

#undef ACCURACY