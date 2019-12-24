#pragma once

#include "effect.h"

class Starfall : public Effect
{
    uint8_t saturation = 150;
    uint8_t density = 60;
    uint8_t tail_step = 100;
public:
    Starfall () {}

    void on_init() {
        set_fps(20);
    }

    void on_update() {
        // заполняем головами комет левую и верхнюю линию
        for (uint8_t i = 0; i <= HEIGHT - 3; i++) {
            if (!getPixColor(i, WIDTH - 1)
                && (random(0, density) == 0)
                && i + 1 < HEIGHT && !getPixColor(i + 1, WIDTH - 1)
                && i - 1 >= 0 && !getPixColor(i - 1, WIDTH - 1)) {
                getPix(i, WIDTH - 1) = CHSV(random(0, 200), saturation, 255);
            }
        }
        for (uint8_t i = 3; i < WIDTH; i++) {
            if (!getPixColor(0, i)
                && (random(0, density) == 0)
                && i + 1 < WIDTH && !getPixColor(0, i + 1)
                && i - 1 >= 0 && !getPixColor(0, i - 1)) {
                getPix(0, i) = CHSV(random(0, 200), saturation, 255);
            }
        }

        // сдвигаем по диагонали
        for (uint8_t x = HEIGHT - 1; x > 0; x--) {
            for (int16_t y = WIDTH - 2; y >= 0; y--) {
                getPix(x, y) = getPixColor(x - 1, y + 1);
            }
        }

        // уменьшаем яркость левой и верхней линии, формируем "хвосты"
        for (uint8_t i = 0; i <= HEIGHT - 3; i++) {
            fadePix(i, WIDTH - 1, tail_step);
        }
        for (uint8_t i = 3; i < WIDTH; i++) {
            fadePix(0, i, tail_step);
        }
    }
};
