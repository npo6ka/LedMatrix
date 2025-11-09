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
    Snow(const std::string& variableStoragePath) :
        Effect(variableStoragePath) {}

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
        // сдвигаем вниз
        for (auto y : LedMatrix.rangeY().reverse()) {
            bool dir = direction;

            for (auto x : LedMatrix.rangeX()) {
                auto& pix = LedMatrix.at(x, y);
                if (pix) {
                    if (y + 1 < LedMatrix.height()) {
                        if (dir) {
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

        for (auto x : LedMatrix.rangeX(0, -2)) {
            // заполняем случайно верхнюю строку
            // а также не даём двум блокам по вертикали вместе быть
            if (!LedMatrix.at(x, 1) && (random8(density) == 0)) {
                LedMatrix.at(x, 0) = 0xE0FFFF - 0x101010 * random8(4);
                x++;
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