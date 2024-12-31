#pragma once

#include "effect_list/effect.h"

#define ACCURACY 100

#define RAINBOW_TICK_SIZE 4 //кол-во тиков до инкремента тика радуги

class HorizontalRainbowPoint : public Effect
{
    int32_t x;
    int32_t y;
    int32_t vec_x;
    int32_t vec_y;
    int32_t tick;
    int32_t min_vec_size = 5;
    int32_t max_vec_size = 10;
    uint8_t tick_size = 4; //кол-во тиков до инкремента тика радуги

private:
    //arg2: horizontal barrier = true or vertical = false
    void rainbow_point_gen_vector(bool horVer) {
        int16_t dir = horVer ? 1 : -1;

        vec_x = (vec_x > 0 ? -dir : dir) * random(min_vec_size, max_vec_size);
        vec_y = (vec_y > 0 ? dir : -dir) * random(min_vec_size, max_vec_size);

        if (vec_y == 0 && vec_x == 0) {
            rainbow_point_gen_vector(horVer);
        }
    }

    void rainbow_point_move_point() {
        x += vec_x;
        y += vec_y;

        if (x < 0) {
            x += ACCURACY * LEDS_WIDTH - 1;
        } else if (x >= ACCURACY * LEDS_WIDTH) {
            x = 0;
        }

        if (y < 0) {
            y = 0;
            rainbow_point_gen_vector(false);
        } else if (y >= ACCURACY * LEDS_HEIGHT) {
            y = ACCURACY * LEDS_HEIGHT - 1;
            rainbow_point_gen_vector(false);
        }  
    }

    int32_t point_distance(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
        return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    void rainbow_point_render_point() {
        for (auto i : LedMatrix.rangeX()) {
            for(auto j : LedMatrix.rangeY()) {
                int32_t loc_x = i * ACCURACY + ACCURACY / 2;
                int32_t loc_y = j * ACCURACY + ACCURACY / 2;

                int32_t dis1 = point_distance(x, y, loc_x, loc_y);
                int32_t dis2 = point_distance(x, y, loc_x - LEDS_WIDTH * ACCURACY, loc_y);
                int32_t dis3 = point_distance(x, y, loc_x + LEDS_WIDTH * ACCURACY, loc_y);

                dis1 = dis1 < dis2 ? dis1 : dis2;
                dis1 = dis1 < dis3 ? dis1 : dis3;

                float chsv = (dis1 / 8 + tick / RAINBOW_TICK_SIZE) % 256;

                LedMatrix.at(i, j) = CHSV(chsv, 255, 255);
            }
        }
    }
public:
    HorizontalRainbowPoint() {}

    void on_init() {
        tick = 0;
        x = random16(0, LEDS_WIDTH * ACCURACY);
        y = random16(0, LEDS_HEIGHT * ACCURACY);

        vec_x = random(0, max_vec_size * 2) - max_vec_size;
        vec_y = random(0, max_vec_size * 2) - max_vec_size;
    }

    void on_update(void) {
        FastLED.clear();

        tick = (tick + 1) % (256 * tick_size);

        rainbow_point_move_point();
        rainbow_point_render_point();
    }
};
