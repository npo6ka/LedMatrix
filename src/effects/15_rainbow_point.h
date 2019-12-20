#pragma once

#define ACCURACY 100
#define MAX_VEC_SIZE 10
#define RAINBOW_TICK_SIZE 4 //кол-во тиков до инкремента тика радуги

#include "effect.h"

class RainbowPoint : public Effect
{
    int32_t x;
    int32_t y;
    int32_t vec_x;
    int32_t vec_y;
    int tick;

    //arg2: horizontal barrier = true or vertical = false
    void rainbow_point_gen_vector(bool horVer) {
        int16_t dir = horVer ? 1 : -1;

        vec_x = (vec_x > 0 ? -dir : dir) * random(0, MAX_VEC_SIZE);
        vec_y = (vec_y > 0 ? dir : -dir) * random(0, MAX_VEC_SIZE);

        if (vec_y == 0 && vec_x == 0) {
            rainbow_point_gen_vector(horVer);
        }
    }

    void rainbow_point_move_point() {
        x += vec_x;
        y += vec_y;

        if (x < 0) {
            x = 0;
            rainbow_point_gen_vector(true);
        } else if (x >= ACCURACY * HEIGHT) {
            x = ACCURACY * HEIGHT - 1;
            rainbow_point_gen_vector(true);
        }

        if (y < 0) {
            y = 0;
            rainbow_point_gen_vector(false);
        } else if (y >= ACCURACY * WIDTH) {
            y = ACCURACY * WIDTH - 1;
            rainbow_point_gen_vector(false);
        }
    }

    void rainbow_point_render_point() {
        int i, j;
        for (i = 0; i < HEIGHT; ++i) {
            for(j = 0; j < WIDTH; ++j) {
                int loc_x = i * ACCURACY + ACCURACY / 2;
                int loc_y = j * ACCURACY + ACCURACY / 2;

                int distance = sqrt((loc_x - x) * (loc_x - x) + (loc_y - y) * (loc_y - y));

                float chsv = (distance / 8 + tick / RAINBOW_TICK_SIZE) % 255;

                getPix(i, j) = CHSV(chsv, 255, 255);
            }
        }
    }

public:
    RainbowPoint() {}

    void on_init()
    {
        tick = 0;
        x = random16(0, (HEIGHT - 1) * ACCURACY);
        y = random16(0, (WIDTH - 1) * ACCURACY);

        vec_x = (int32_t)random(0, MAX_VEC_SIZE * 2) - MAX_VEC_SIZE;
        vec_y = (int32_t)random(0, MAX_VEC_SIZE * 2) - MAX_VEC_SIZE;

        set_fps(60);
    }

    void on_update()
    {
        FastLED.clear();

        tick = (tick + 1) % (256 * RAINBOW_TICK_SIZE);

        rainbow_point_move_point();
        rainbow_point_render_point();
    }
};
