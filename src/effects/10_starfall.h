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
        set_fps(30);
    }

    void on_update() {
        // заполняем головами комет левую и верхнюю линию
        for (uint8_t i = HEIGHT / 2; i < HEIGHT; i++) {
            if (!getPixColor(0, i)
                && (random(0, density) == 0)
                && i + 1 < HEIGHT && !getPixColor(0, i + 1)
                && i - 1 >= 0 && !getPixColor(0, i - 1)) {
                setPixColor(0, i, CHSV(random(0, 200), saturation, 255));
            }
        }
        for (uint8_t i = 0; i < WIDTH / 2; i++) {
            if (!getPixColor(i, HEIGHT - 1)
                && (random(0, density) == 0)
                && i + 1 < HEIGHT && !getPixColor(i + 1, HEIGHT - 1)
                && i - 1 >= 0 && !getPixColor(i - 1, HEIGHT - 1)) {
                setPixColor(i, HEIGHT - 1, CHSV(random(0, 200), saturation, 255));
            }
        }

        // сдвигаем по диагонали
        for (uint8_t y = 0; y < HEIGHT - 1; y++) {
            for (uint8_t x = WIDTH - 1; x > 0; x--) {
                setPixColor(x, y, getPixColor(x - 1, y + 1));
            }
        }

        // уменьшаем яркость левой и верхней линии, формируем "хвосты"
        for (uint8_t i = HEIGHT / 2; i < HEIGHT; i++) {
            fadePix(0, i, tail_step);
        }
        for (uint8_t i = 0; i < WIDTH / 2; i++) {
            fadePix(i, HEIGHT - 1, tail_step);
        }
    }
};
