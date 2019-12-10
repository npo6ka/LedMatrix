#pragma once

#define data ((RandomRainData *)global_data)

struct RandomRainData
{
    uint8_t step;
};

void random_rain_prepare()
{
    data->step = 2;
    eff_set_ups(30);
}

void random_rain_update()
{
  int i, j;

  for (i = 0; i < WIDTH; ++i) {
    for (j = 0; j < HEIGHT; ++j) {
      fadePix(i, j, data->step);

      CRGB cl = getPix(i, j);

      if (random16(500) == 0) {
        int asd = 0;
        cl.r = MAX_BRIGHTNESS;
      }
      if (random16(500) == 0) {
        int asd = 0;
        cl.g = MAX_BRIGHTNESS;
      }
      if (random16(500) == 0) {
        int asd = 0;
        cl.b = MAX_BRIGHTNESS;
      }

      setPixColor(i, j, cl);
   }
  }
}

#undef data
