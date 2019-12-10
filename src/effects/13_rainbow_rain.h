#pragma once

#define data ((RainbowRainData *)global_data)

#define RAINBOW_TICK_SIZE 4 //кол-во тиков до инкремента тика радуги

struct RainbowRainData
{
    uint8_t step;
    int tick;
};

void rainbow_rain_prepare()
{
    data->step = 2;
    data->tick = 0;
    eff_set_ups(40);
}

void rainbow_rain_update()
{
    int i, j;

    for (i = 0; i < WIDTH; ++i) {
      for (j = 0; j < HEIGHT; ++j) {
        fadePix(i, j, data->step);

        CRGB cl = getPix(i, j);

        if (random8(200) == 0) {
            int asd = 0;
            cl = CHSV(data->tick / RAINBOW_TICK_SIZE, 255, 255);
        }

        setPixColor(i, j, cl);
      }
    }

    data->tick = (data->tick + 1) % ((MAX_HSV + 1) * RAINBOW_TICK_SIZE);
}

#undef data
