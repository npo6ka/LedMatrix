#pragma once

#include "effect.h"

class TheMatrix : public Effect
{
    uint8_t fade = 0x30;

public:
    TheMatrix() {}

    void on_init() {
        set_fps(30);
    }

    void on_update() {
        // сдвигаем всё вниз
        for (uint8_t x = HEIGHT - 1; x > 0 ; --x) {
            for (uint8_t y = 0; y < WIDTH; ++y) {
                getPix(x, y) = getPixColor(x - 1, y);
            }
        }

        for (uint8_t y = 0; y < WIDTH; y++) {
            uint32_t thisColor = getPixColor(0, y);

            if (thisColor == 0)
                // заполняем случайно верхнюю строку
                getPix(0, y) = 0x00FF00 * (random(0, 10) == 0);
            else
                // или просто делайем фейд пикеля
                fadePix(0, y, fade);
        }
    }
};
