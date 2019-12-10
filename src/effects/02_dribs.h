#pragma once

#define data ((DribsData *)global_data)

struct DribsData
{
    uint8_t cur_drib[WIDTH];
    uint8_t lenght;
};

void dribs_prepare()
{
    data->lenght = 12;

    eff_set_ups(20);
}

void dribs_update()
{
    for (int i = 0; i < WIDTH;  ++i) {
        if (data->cur_drib[i] == 0 && random8(40) == 0) {
            data->cur_drib[i] = 1;
        } else if (data->cur_drib[i] != 0) {
            data->cur_drib[i] += 1;
            if (data->cur_drib[i] >= data->lenght + HEIGHT) {
                data->cur_drib[i] = 0;
            }
        }
    }

    int step = MAX_BRIGHTNESS / data->lenght;
    for (int i = 0; i < WIDTH;  ++i) {
        for (int j = 0; j < HEIGHT;  ++j) {
            if (j < data->cur_drib[i] && data->cur_drib[i] != 0) {
                getPix(i, j) = CRGB(0, 0, max(MAX_BRIGHTNESS - (data->cur_drib[i] - j - 1) * step, 0));
            } else {
                getPix(i, j) = 0x0;
            }
        }
    }
}

#undef data
