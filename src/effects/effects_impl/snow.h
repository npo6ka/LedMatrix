#pragma once

#include "effects/effect.h"
#include "libs/led_matrix.h"

class Snow : public Effect
{
    uint8_t step;
    uint8_t tick;
    uint8_t density;
    bool direction;
public:
    Snow() {}

    void on_init()
    {
        step = 20;
        density = 10;
        tick = 0;
        direction = false;
        set_fps(60);
    }

    void move_snow()
    {
        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();

        // сдвигаем вниз
        for (auto y : LedMatrix.rangeY().reverse()) {
            bool dir = direction;

            for (auto x : LedMatrix.rangeX()) {
                auto& pix = LedMatrix.at(x, y);
                if (pix) {
                    if (y + 1 < h) {
                        if (w <= 1) {
                            LedMatrix.at(x, y + 1) = pix;
                        } else if (dir) {
                            LedMatrix.at(x + 1, y + 1) = pix;
                        } else {
                            LedMatrix.at(x - 1, y + 1) = pix;
                        }
                        dir = !dir;
                    }

                    pix = 0x0;
                }
            }
        }

        const int right = (w > 2) ? -2 : -1;
        for (auto x : LedMatrix.rangeX(0, right)) {
            // заполняем случайно верхнюю строку
            // а также не даём двум блокам по вертикали вместе быть
            index_t check_y = (h > 1) ? 1 : 0;
            if (!LedMatrix.at(x, check_y) && (random8(density) == 0)) {
                LedMatrix.at(x, 0) = 0xE0FFFF - 0x101010 * random8(4);
                if (w > 1) {
                    x++;
                }
            } else {
                LedMatrix.at(x, 0) = 0x000000;
            }
        }
    }

    void on_update()
    {
        if (tick >= step) {
            direction = !direction;
            move_snow();
            tick = 0;
        } else {
            tick++;
        }
    }
};
