#pragma once

#include "effects/effect.h"

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
        for (auto x : LedMatrix.rangeX()) {
            for (auto y : LedMatrix.rangeY(1).reverse()) {
                LedMatrix.at(x, y) = LedMatrix.at(x, y - 1);
            }
        }

        for (auto x : LedMatrix.rangeX()) {
            auto& pix = LedMatrix.at(x, 0);
            if (!pix) {
                // заполняем случайно верхнюю строку
                pix = 0x00FF00 * (random8(0, 100) == 0);
            } else {
                // или просто делайем фейд пикеля
                pix.fadeToBlackBy(fade);
            }
        }
    }
};
