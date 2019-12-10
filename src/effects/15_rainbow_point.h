#pragma once

#define data ((RainbowPointData *)global_data)

#define ACCURACY 100
#define MAX_VEC_SIZE 10
#define RAINBOW_TICK_SIZE 4 //кол-во тиков до инкремента тика радуги

struct RainbowPointData
{
    int32_t x;
    int32_t y;
    int32_t vec_x;
    int32_t vec_y;
    int tick;
};

//arg2: horizontal barrier = true or vertical = false
void rainbow_point_gen_vector(bool horVer) {
    int16_t dir = horVer ? 1 : -1;

    data->vec_x = (data->vec_x > 0 ? -dir : dir) * random(0, MAX_VEC_SIZE);
    data->vec_y = (data->vec_y > 0 ? dir : -dir) * random(0, MAX_VEC_SIZE);

    if (data->vec_y == 0 && data->vec_x == 0) {
        rainbow_point_gen_vector(horVer);
    }
}

void rainbow_point_move_point() {
    data->x += data->vec_x;
    data->y += data->vec_y;

    if (data->x < 0) {
        data->x = 0;
        rainbow_point_gen_vector(true);
    } else if (data->x >= ACCURACY * WIDTH) {
        data->x = ACCURACY * WIDTH - 1;
        rainbow_point_gen_vector(true);
    }

    if (data->y < 0) {
        data->y = 0;
        rainbow_point_gen_vector(false);
    } else if (data->y >= ACCURACY * HEIGHT) {
        data->y = ACCURACY * HEIGHT - 1;
        rainbow_point_gen_vector(false);
    }
}

void rainbow_point_render_point() {
    int i, j;
    for (i = 0; i < WIDTH; ++i) {
        for(j = 0; j < HEIGHT; ++j) {
            int loc_x = i * ACCURACY + ACCURACY / 2;
            int loc_y = j * ACCURACY + ACCURACY / 2;

            int distance = sqrt((loc_x - data->x) * (loc_x - data->x) + (loc_y - data->y) * (loc_y - data->y));

            float chsv = (distance / 8 + data->tick / RAINBOW_TICK_SIZE) % MAX_HSV;

            setPixColor(i, j, CHSV(chsv, 255, 255));
        }
    }
}

void rainbow_point_prepare()
{
    data->tick = 0;
    data->x = random16(0, (WIDTH - 1) * ACCURACY);
    data->y = random16(0, (HEIGHT - 1) * ACCURACY);

    data->vec_x = (int32_t)random(0, MAX_VEC_SIZE * 2) - MAX_VEC_SIZE;
    data->vec_y = (int32_t)random(0, MAX_VEC_SIZE * 2) - MAX_VEC_SIZE;

    eff_set_ups(60);
}

void rainbow_point_update()
{
    FastLED.clear();

    int i;

    data->tick = (data->tick + 1) % ((MAX_HSV + 1) * RAINBOW_TICK_SIZE);

    rainbow_point_move_point();
    rainbow_point_render_point();
}

#undef data
