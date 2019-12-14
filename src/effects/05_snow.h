#pragma once

#include "effect.h"

class Snow : public Effect
{
    uint8_t step;
    uint8_t tick;
    int density;
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

    void on_update()
    {
        if (tick >= step) {
            direction = !direction;
            // сдвигаем вниз
            for (int8_t x = 0; x < HEIGHT; x++) {
                bool dir = direction;

                for (uint8_t y = 0; y < WIDTH; y++) {
                    if (getPixColor(x, y)) {
                        if (x - 1 >= 0) {
                            if (dir) {
                                if (y + 1 < WIDTH) setPixColor(x - 1, y + 1, getPixColor(x, y));
                            } else {
                                if (y - 1 >= 0) setPixColor(x - 1, y - 1, getPixColor(x, y));
                            }
                            dir = !dir;
                        }

                        setPixColor(x, y, 0);
                    }
                }
            }

            for (uint8_t x = 0; x < WIDTH - 1; x++) {
                // заполняем случайно верхнюю строку
                // а также не даём двум блокам по вертикали вместе быть
                if (getPixColor(HEIGHT - 2, x) == 0 && (random(0, density) == 0)) {
                    setPixColor(HEIGHT - 1, x, 0xE0FFFF - 0x101010 * random(0, 4));
                    x++;
                } else {
                    setPixColor(HEIGHT - 1, x, 0x000000);
                }
            }

            tick = 0;
        } else {
            tick++;
        }
    }
};