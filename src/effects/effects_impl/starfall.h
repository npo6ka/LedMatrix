#pragma once

#include "effects/effect.h"

class Starfall : public Effect
{
    uint8_t saturation = 150;
    uint8_t density = 60;
    uint8_t tail_step = 100;
    uint8_t border = 2;
public:
    Starfall () {}

    void on_init() {
        set_fps(20);
    }

    void on_update() {
        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();

        // если ширина матрицы меньше 2
        if (w <= 1) {
            if (h > 1) {
                for (auto y : LedMatrix.rangeY(1).reverse()) {
                    LedMatrix.at(0, y) = LedMatrix.at(0, y - 1);
                }
            }
            if (!LedMatrix.at(0, 0) && (random8(0, density) == 0)) {
                LedMatrix.at(0, 0) = CHSV(random8(0, 200), saturation, 255);
            }
            LedMatrix.at(0, 0).fadeToBlackBy(tail_step);
            return;
        }

        if (h <= 1) {
            if (w > 1) {
                for (auto x : LedMatrix.rangeX(1).reverse()) {
                    LedMatrix.at(x, 0) = LedMatrix.at(x - 1, 0);
                }
            }
            if (!LedMatrix.at(0, 0) && (random8(0, density) == 0)) {
                LedMatrix.at(0, 0) = CHSV(random8(0, 200), saturation, 255);
            }
            LedMatrix.at(0, 0).fadeToBlackBy(tail_step);
            return;
        }

        const int x_border = (w > border) ? -(int)border : -1;
        const int y_border = (h > border) ? -(int)border : -1;

        // заполняем головами комет левую и верхнюю линию
        for (auto x : LedMatrix.rangeX(0, x_border)) {
            if (!LedMatrix.at(x, 0)
                && (random8(0, density) == 0)
                && !LedMatrix.at(x + 1, 0)
                && !LedMatrix.at(x - 1, 0)) {
                LedMatrix.at(x, 0) = CHSV(random8(0, 200), saturation, 255);
            }
        }
        for (auto y : LedMatrix.rangeY(0, y_border)) {
            if (!LedMatrix.at(0, y)
                && (random8(0, density) == 0)
                && !LedMatrix.at(0, y + 1)
                && !LedMatrix.at(0, y - 1)) {
                LedMatrix.at(0, y) = CHSV(random8(0, 200), saturation, 255);
            }
        }

        // сдвигаем по диагонали
        for (auto x : LedMatrix.rangeX(1).reverse()) {
            for (auto y : LedMatrix.rangeY(1).reverse()) {
                LedMatrix.at(x, y) = LedMatrix.at(x - 1, y - 1);
            }
        }

        // уменьшаем яркость левой и верхней линии, формируем "хвосты"
        for (auto x : LedMatrix.rangeX(0, x_border)) {
            LedMatrix.at(x, 0).fadeToBlackBy(tail_step);
        }
        for (auto y : LedMatrix.rangeY(0, y_border)) {
            LedMatrix.at(0, y).fadeToBlackBy(tail_step);
        }
    }
};
