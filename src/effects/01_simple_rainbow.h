#pragma once

#define data ((SimpleRaibowData *)global_data)

struct SimpleRaibowData
{
    int tick;
    int phaseShift;
};

void simple_rainbow_prepare()
{
    data->tick = 0;
    data->phaseShift = 10;

    eff_set_ups(20);
}

void simple_rainbow_update()
{
    uint8_t x, y;
    data->tick = data->tick % (MAX_HSV + 1);

    for (x = 0; x < 10; x++) {
        for (y = 0; y < 10; y++) {
            getPix(x, y) = CHSV((data->tick + x + y * data->phaseShift / 2) % (MAX_HSV + 1), 255, 255);
        }
    }

    data->tick += 1;
}

#undef data
