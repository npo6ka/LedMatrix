#pragma once

#define data ((PointsData *)global_data)

#define ACCURACY 100
#define MAX_VEC_SIZE 10
#define POINTS_AMNT 3

typedef struct Point
{
  int32_t x;
  int32_t y;
  int32_t vec_x;
  int32_t vec_y;
  CRGB color;
} Point;

struct PointsData
{
    uint32_t point_size;
    uint32_t bright_radius;
    Point points[POINTS_AMNT];
    int tick;
};

//arg2: horizontal barrier = true or vertical = false
static void gen_vector(Point &pnt, bool horVer)
{
  int16_t dir = horVer ? 1 : -1;

  pnt.vec_x = (pnt.vec_x > 0 ? -dir : dir) * random(0, MAX_VEC_SIZE);
  pnt.vec_y = (pnt.vec_y > 0 ? dir : -dir) * random(0, MAX_VEC_SIZE);

  if (pnt.vec_y == 0 && pnt.vec_x == 0) {
    gen_vector(pnt, horVer);
  }
}

static void move_point(Point &pnt)
{
  pnt.x += pnt.vec_x;
  pnt.y += pnt.vec_y;

  if (pnt.x < 0) {
    pnt.x = 0;
    gen_vector(pnt, true);
  } else if (pnt.x >= ACCURACY * WIDTH) {
    pnt.x = ACCURACY * WIDTH - 1;
    gen_vector(pnt, true);
  }

  if (pnt.y < 0) {
    pnt.y = 0;
    gen_vector(pnt, false);
  } else if (pnt.y >= ACCURACY * HEIGHT) {
    pnt.y = ACCURACY * HEIGHT - 1;
    gen_vector(pnt, false);
  }
}

// return float val in range 0 .. 1
static float get_func_brithtness(int distance)
{
  if (distance <= data->point_size) {
    return 1;
  } else if (distance >= data->point_size + data->bright_radius) {
    return 0;
  } else {
  float val = (float)(distance - data->point_size) / data->bright_radius;
  return ( 7.3890560 /* e^2 */ ) / (50 * val + 7) - 0.13;
  }
}

static void render_point(Point pnt)
{
  int i, j;
  for (i = 0; i < WIDTH; ++i) {
    for(j = 0; j < HEIGHT; ++j) {
      int loc_x = i * ACCURACY + ACCURACY / 2;
      int loc_y = j * ACCURACY + ACCURACY / 2;

      int distance = sqrt((loc_x - pnt.x) * (loc_x - pnt.x) + (loc_y - pnt.y) * (loc_y - pnt.y));

      float bright = get_func_brithtness(distance);
      CRGB clr = getPix(i, j);
      clr.r = qadd8(clr.r, (float)pnt.color.r * bright);
      clr.g = qadd8(clr.g, (float)pnt.color.g * bright);
      clr.b = qadd8(clr.b, (float)pnt.color.b * bright);
      setPixColor(i, j, clr);
    }
  }
}

void points_prepare()
{
    int i;
    data->tick = 0;
    data->point_size = 50;
    data->bright_radius = 400;

    for (i = 0; i < POINTS_AMNT; ++i) {
        data->points[i].x = random16(0, (WIDTH - 1) * ACCURACY);
        data->points[i].y = random16(0, (HEIGHT - 1) * ACCURACY);

        data->points[i].vec_x = (int32_t)random(0, MAX_VEC_SIZE * 2) - MAX_VEC_SIZE;
        data->points[i].vec_y = (int32_t)random(0, MAX_VEC_SIZE * 2) - MAX_VEC_SIZE;
        data->points[i].color = CHSV(random8(), 255, 255);
    }

    if (POINTS_AMNT >= 3) {
      data->points[0].color = 0xff;
      data->points[1].color = 0xff00;
      data->points[2].color = 0xff0000;
    }

    eff_set_ups(60);
}

void points_update()
{
    FastLED.clear();

    int i;

    data->tick++;
    for (i = 0; i < POINTS_AMNT; ++i) {
      move_point(data->points[i]);
      render_point(data->points[i]);
    }
}

#undef data
