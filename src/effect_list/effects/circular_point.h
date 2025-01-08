#pragma once

#include "effect_list/effect.h"

#define ACCURACY 10

class CircularPoint : public Effect
{
    struct Point
    {
        int32_t x;
        int32_t y;
        uint8_t hue;
        uint32_t pr; //point_radius
        uint32_t br; //bright_radius
    };

    uint8_t tick;
    Point p1;
    uint32_t v;
    uint8_t rainbow_tick_size;

private:
    static float get_func_brithtness(uint32_t distance, Point pnt)
    {
        if (distance <= pnt.pr) {
            return 1;
        } else if (distance >= pnt.pr + pnt.br) {
            return 0;
        } else {
            float val = (float)(distance - pnt.pr) / pnt.br;
            return ( 7.389056f /* e^2 */ ) / (50 * val + 7) - 0.13f;
        }
    }

    static void render_point(Point pnt)
    {
      for (auto i : LedMatrix.rangeX()) {
        for (auto j : LedMatrix.rangeY()) {
          int loc_x = i * ACCURACY + ACCURACY / 2;
          int loc_y = j * ACCURACY + ACCURACY / 2;

          uint32_t distance = sqrt((loc_x - pnt.x) * (loc_x - pnt.x) + (loc_y - pnt.y) * (loc_y - pnt.y));

          float bright = get_func_brithtness(distance, pnt) * 0.7f;
          CRGB &clr = LedMatrix.at(i, j);
          CRGB pnt_clr = CHSV(pnt.hue, 255, 255);
          clr.r = qadd8(clr.r, (float)pnt_clr.r * bright);
          clr.g = qadd8(clr.g, (float)pnt_clr.g * bright);
          clr.b = qadd8(clr.b, (float)pnt_clr.b * bright);
        }
      }
    }

public:
    CircularPoint() {}

    void on_init() {
        tick = 0;
        v = 1;

        p1.hue = 0;
        p1.pr = (min(LEDS_HEIGHT, LEDS_WIDTH) / 4 + 1) * ACCURACY;
        p1.br = 0;
        rainbow_tick_size = 1;
    }

    //tick 0 .. 255 -> 0 .. 2 * pi
    float get_pi_tick(uint8_t tick) {
        return 3.141592f * 2 * tick / 255;
    }

    void on_update(void) {
        float angle = get_pi_tick(tick);
        float move_radius = 0.7f;// 0.7 .. 0.9
        p1.x = ACCURACY * LEDS_WIDTH * (move_radius * cos(angle) + 1) / 2;
        p1.y = ACCURACY * LEDS_HEIGHT * (move_radius * sin(angle) + 1) / 2;
        p1.hue += rainbow_tick_size;

        LedMatrix.fader(5);
        render_point(p1);
        tick = tick + 3;
    }
};

#undef ACCURACY