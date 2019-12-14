#pragma once

#include "effect.h"

class TheMatrix : public Effect
{

public:
    TheMatrix() {}

    void on_init() {
        set_fps(30);
    }

    void on_update() {
        for (uint8_t x = 0; x < WIDTH; x++) {
        // заполняем случайно верхнюю строку
        uint32_t thisColor = getPixColor(x, HEIGHT - 1);
        if (thisColor == 0)
            setPixColor(x, HEIGHT - 1, 0x00FF00 * (random(0, 10) == 0));
        else if (thisColor < 0x002000)
            setPixColor(x, HEIGHT - 1, 0);
        else
            setPixColor(x, HEIGHT - 1, thisColor - 0x002000);
        }

        // сдвигаем всё вниз
        for (uint8_t x = 0; x < WIDTH; x++) {
            for (uint8_t y = 0; y < HEIGHT - 1; y++) {
                setPixColor(x, y, getPixColor(x, y + 1));
            }
        }
    }
};
