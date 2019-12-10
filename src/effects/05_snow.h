#pragma once

#define data ((SnowData *)global_data)

struct SnowData
{
    uint8_t step;
    uint8_t tick;
    int density;
    bool direction;
};

void snow_prepare()
{
    data->step = 20;
    data->density = 10;
    data->tick = 0;
    data->direction = false;
    eff_set_ups(60);
}

void snow_update()
{
    if (data->tick >= data->step) {
        data->direction = !data->direction;
        // сдвигаем вниз
        for (int8_t y = HEIGHT - 1; y >= 0; y--) {
            bool dir = data->direction;

            for (uint8_t x = 0; x < WIDTH; x++) {
                if (getPixColor(x, y)) {
                    if (y + 1 < HEIGHT) {
                        if (dir) {
                            setPixColor(x + 1, y + 1, getPixColor(x, y));
                        } else {
                            setPixColor(x - 1, y + 1, getPixColor(x, y));
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
            if (getPixColor(x, 1) == 0 && (random(0, data->density) == 0)) {
                setPixColor(x, 0, 0xE0FFFF - 0x101010 * random(0, 4));
                x++;
            } else {
                setPixColor(x, 0, 0x000000);
            }
        }

        data->tick = 0;
    } else {
        data->tick++;
    }
}

#undef data
