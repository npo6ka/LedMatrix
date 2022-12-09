#pragma once

#include "effect.h"

#define ACCURACY 100
#define MAX_VEC_SIZE 20
#define POINTS_AMNT 3

typedef struct Point
{
  int32_t x;
  int32_t y;
  int32_t vec_x;
  int32_t vec_y;
  CRGB color;
} Point;

class Points : public Effect
{
    uint32_t point_size;
    uint32_t bright_radius;
    Point points[POINTS_AMNT];
    int tick;

    //arg2: horizontal barrier = true or vertical = false
    void gen_vector(Point &pnt, bool horVer)
    {
        int16_t dir = horVer ? 1 : -1;

        pnt.vec_x = (pnt.vec_x > 0 ? -dir : dir) * random(0, MAX_VEC_SIZE);
        pnt.vec_y = (pnt.vec_y > 0 ? dir : -dir) * random(0, MAX_VEC_SIZE);

        if (pnt.vec_y == 0 && pnt.vec_x == 0) {
            gen_vector(pnt, horVer);
        }
    }

    void move_point(Point &pnt)
    {
        pnt.x += pnt.vec_x;
        pnt.y += pnt.vec_y;

        if (pnt.x < 0) {
            pnt.x = 0;
            gen_vector(pnt, true);
        } else if (pnt.x >= ACCURACY * HEIGHT) {
            pnt.x = ACCURACY * HEIGHT - 1;
            gen_vector(pnt, true);
        }

        if (pnt.y < 0) {
            pnt.y = 0;
            gen_vector(pnt, false);
        } else if (pnt.y >= ACCURACY * WIDTH) {
            pnt.y = ACCURACY * WIDTH - 1;
            gen_vector(pnt, false);
        }
    }

    // return float val in range 0 .. 1
    float get_func_brithtness(uint32_t distance)
    {
        if (distance <= point_size) {
            return 1;
        } else if (distance >= point_size + bright_radius) {
            return 0;
        } else {
            float val = (float)(distance - point_size) / bright_radius;
            return ( 7.3890560 /* e^2 */ ) / (50 * val + 7) - 0.13;
        }
    }

    void render_point(Point pnt)
    {
        int i, j;
        for (i = 0; i < HEIGHT; ++i) {
            for(j = 0; j < WIDTH; ++j) {
                int loc_x = i * ACCURACY + ACCURACY / 2;
                int loc_y = j * ACCURACY + ACCURACY / 2;

                int distance = sqrt((loc_x - pnt.x) * (loc_x - pnt.x) + (loc_y - pnt.y) * (loc_y - pnt.y));

                float bright = get_func_brithtness(distance);
                CRGB &clr = getPix(i, j);
                clr.r = qadd8(clr.r, (float)pnt.color.r * bright);
                clr.g = qadd8(clr.g, (float)pnt.color.g * bright);
                clr.b = qadd8(clr.b, (float)pnt.color.b * bright);
            }
        }
    }

public:
    Points() {}

    void on_init()
    {
        int i;
        tick = 0;
        point_size = 50;
        bright_radius = 400;

        for (i = 0; i < POINTS_AMNT; ++i) {
            points[i].x = random16(0, (HEIGHT - 1) * ACCURACY);
            points[i].y = random16(0, (WIDTH - 1) * ACCURACY);

            points[i].vec_x = (int32_t)random(0, MAX_VEC_SIZE * 2) - MAX_VEC_SIZE;
            points[i].vec_y = (int32_t)random(0, MAX_VEC_SIZE * 2) - MAX_VEC_SIZE;
            points[i].color = CHSV(random8(), 255, 255);
        }

        if (POINTS_AMNT >= 3) {
            points[0].color = 0x0000ff;
            points[1].color = 0x00ff00;
            points[2].color = 0xff0000;
        }

        set_fps(60);
    }

    void on_update()
    {
        FastLED.clear();

        int i;

        tick++;
        for (i = 0; i < POINTS_AMNT; ++i) {
            move_point(points[i]);
            render_point(points[i]);
        }
    }
};