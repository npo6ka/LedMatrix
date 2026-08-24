#pragma once

#include "Arduino.h"
#include "effects/effect.h"

class RainbowStaticPoint : public Effect
{
    static constexpr int32_t ACCURACY = 10;

    int tick;
    uint8_t tick_size = 1; //кол-во тиков до инкремента тика радуги
    uint16_t hue_period = random8(16, 255); //длина полного цикла радуги в пикселях: больше — плавнее переход оттенка

    void rainbow_static_point_render_point() {
        int x = LedMatrix.width() / 2 * ACCURACY;
        int y = LedMatrix.height() / 2 * ACCURACY;

        for (auto i : LedMatrix.rangeX()) {
            for(auto j : LedMatrix.rangeY()) {
                int loc_x = i * ACCURACY + ACCURACY / 2;
                int loc_y = j * ACCURACY + ACCURACY / 2;

                int distance = sqrt((loc_x - x) * (loc_x - x) + (loc_y - y) * (loc_y - y));

                int32_t hue = (distance * 256 / ((int32_t)hue_period * ACCURACY) + tick / tick_size) % 256;

                LedMatrix.at(i, j) = CHSV((uint8_t)hue, 255, 255);
            }
        }
    }

public:
    RainbowStaticPoint() {}

    void on_init()
    {
        tick = 0;
        hue_period = random8(16, 255);
        set_fps(60);
    }

    void on_update()
    {
        tick = (tick + 4) % (256 * tick_size);
        rainbow_static_point_render_point();
    }
};