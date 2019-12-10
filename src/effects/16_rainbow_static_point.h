#pragma once

#define data ((RainbowStaticPointData *)global_data)

#define ACCURACY 10
#define RAINBOW_TICK_SIZE 4 //кол-во тиков до инкремента тика радуги

struct RainbowStaticPointData
{
    int tick;
};

static void rainbow_static_point_render_point() {
    int i, j;
    int x = WIDTH / 2 * ACCURACY;
    int y = HEIGHT / 2 * ACCURACY;

    for (i = 0; i < WIDTH; ++i) {
        for(j = 0; j < HEIGHT; ++j) {
            int loc_x = i * ACCURACY + ACCURACY / 2;
            int loc_y = j * ACCURACY + ACCURACY / 2;

            int distance = sqrt((loc_x - x) * (loc_x - x) + (loc_y - y) * (loc_y - y));

            float chsv = (distance + data->tick / RAINBOW_TICK_SIZE) % MAX_HSV;

            setPixColor(i, j, CHSV(chsv, 255, 255));
        }
    }
}

void rainbow_static_point_prepare()
{
    data->tick = 0;
    eff_set_ups(60);
}

void rainbow_static_point_update()
{
    data->tick = (data->tick + 1) % ((MAX_HSV + 1) * RAINBOW_TICK_SIZE);
    rainbow_static_point_render_point();
}

#undef data
