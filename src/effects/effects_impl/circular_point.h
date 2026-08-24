#pragma once

#include "effects/effect.h"

class CircularPoint : public Effect
{
    static constexpr int32_t ACCURACY = 10;

    struct Point
    {
        int32_t x;
        int32_t y;
        uint8_t hue;
        uint32_t pr; //point_radius
        uint32_t br; //bright_radius
    };

    static constexpr uint8_t tick_step = 2;    //прирост tick за кадр, задаёт скорость обхода круга
    static constexpr float move_radius = 1.0f; //доля хода до краёв матрицы: 1.0 — точка достаёт до крайних диодов
    static constexpr uint8_t fade_step = 20;   //затухание шлейфа за кадр: больше — короче хвост

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
      const int32_t reach = pnt.pr + pnt.br;
      const CRGB pnt_clr = CHSV(pnt.hue, 255, 255);

      int32_t x_from = (pnt.x - reach) / ACCURACY;
      int32_t x_to   = (pnt.x + reach) / ACCURACY;
      int32_t y_from = (pnt.y - reach) / ACCURACY;
      int32_t y_to   = (pnt.y + reach) / ACCURACY;

      if (x_from < 0) x_from = 0;
      if (y_from < 0) y_from = 0;
      if (x_to > (int32_t)LedMatrix.width() - 1) x_to = (int32_t)LedMatrix.width() - 1;
      if (y_to > (int32_t)LedMatrix.height() - 1) y_to = (int32_t)LedMatrix.height() - 1;

      for (int32_t i = x_from; i <= x_to; ++i) {
        for (int32_t j = y_from; j <= y_to; ++j) {
          int loc_x = i * ACCURACY + ACCURACY / 2;
          int loc_y = j * ACCURACY + ACCURACY / 2;

          uint32_t distance = sqrt((loc_x - pnt.x) * (loc_x - pnt.x) + (loc_y - pnt.y) * (loc_y - pnt.y));

          float bright = get_func_brithtness(distance, pnt) * 0.7f;
          CRGB &clr = LedMatrix.at((index_t)i, (index_t)j);
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

        const int32_t w = (int32_t)LedMatrix.width();
        const int32_t h = (int32_t)LedMatrix.height();

        int32_t radius = ((w < h ? w : h) / 4 + 1) * ACCURACY;

        // за кадр точка сдвигается на amp * шаг угла; если сдвиг больше её радиуса,
        // между кадрами остаются неосвещённые диоды
        const int32_t amp_x = move_radius * ACCURACY * (w - 1) / 2;
        const int32_t amp_y = move_radius * ACCURACY * (h - 1) / 2;
        const int32_t amp = amp_x > amp_y ? amp_x : amp_y;
        const int32_t frame_shift = amp * 2 * 3.141592f * tick_step / 255;

        if (radius < frame_shift) {
            radius = frame_shift;
        }

        p1.hue = 0;
        p1.pr = radius;
        p1.br = radius;
        rainbow_tick_size = 1;
        set_fps(60);
    }

    //tick 0 .. 255 -> 0 .. 2 * pi
    float get_pi_tick(uint8_t tick) {
        return 3.141592f * 2 * tick / 255;
    }

    void on_update(void) {
        float angle = get_pi_tick(tick);
        // ход считается по центрам крайних пикселей, иначе точка упирается в границу раньше края ленты
        p1.x = ACCURACY / 2 + ACCURACY * ((int32_t)LedMatrix.width() - 1) * (move_radius * cos(angle) + 1) / 2;
        p1.y = ACCURACY / 2 + ACCURACY * ((int32_t)LedMatrix.height() - 1) * (move_radius * sin(angle) + 1) / 2;
        p1.hue += rainbow_tick_size;

        LedMatrix.fader(fade_step);
        render_point(p1);
        tick = tick + tick_step;
    }
};