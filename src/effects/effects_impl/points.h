#pragma once

#include "effects/effect.h"

class Points : public Effect
{
    static constexpr int32_t ACCURACY = 10;
    static constexpr int32_t MAX_VEC_SIZE = 10;
    static constexpr int POINTS_AMNT = 20;
    static constexpr int MAX_BRIGHT_RADIUS = 64;

    struct Point {
        int32_t x;
        int32_t y;
        int32_t vec_x;
        int32_t vec_y;
        CRGB color;
    };

    int32_t point_size;
    int32_t bright_radius;
    // яркость свечения по расстоянию от края точки, считается один раз в on_init
    uint8_t bright_table[MAX_BRIGHT_RADIUS];
    Point points[POINTS_AMNT];
    int tick;

    //arg2: horizontal barrier = true or vertical = false
    void gen_vector(Point &pnt, bool horVer)
    {
        int16_t dir = horVer ? 1 : -1;

        pnt.vec_x = (pnt.vec_x > 0 ? -dir : dir) * random(MAX_VEC_SIZE / 4, MAX_VEC_SIZE);
        pnt.vec_y = (pnt.vec_y > 0 ? dir : -dir) * random(MAX_VEC_SIZE / 4, MAX_VEC_SIZE);

        if (pnt.vec_y == 0 && pnt.vec_x == 0) {
            gen_vector(pnt, horVer);
        }
    }

    void move_point(Point &pnt)
    {
        pnt.x += pnt.vec_x;
        pnt.y += pnt.vec_y;

        const int32_t limit_x = ACCURACY * LedMatrix.width();
        const int32_t limit_y = ACCURACY * LedMatrix.height();

        if (pnt.x < 0) {
            pnt.x = 0;
            gen_vector(pnt, true);
        } else if (pnt.x >= limit_x) {
            pnt.x = limit_x - 1;
            gen_vector(pnt, true);
        }

        if (pnt.y < 0) {
            pnt.y = 0;
            gen_vector(pnt, false);
        } else if (pnt.y >= limit_y) {
            pnt.y = limit_y - 1;
            gen_vector(pnt, false);
        }
    }

    // return float val in range 0 .. 1
    float get_func_brithtness(int32_t distance)
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

    uint8_t get_brithtness(uint16_t distance)
    {
        if (distance <= point_size) {
            return 255;
        }
        const int32_t idx = distance - point_size;
        if (idx >= bright_radius) {
            return 0;
        }
        return bright_table[idx];
    }

    void render_point(const Point &pnt)
    {
        // за пределами reach точка уже не светит, вся матрица не нужна
        const int32_t reach = point_size + bright_radius;
        const int32_t reach2 = reach * reach;

        int32_t x_from = (pnt.x - reach) / ACCURACY;
        int32_t x_to   = (pnt.x + reach) / ACCURACY;
        int32_t y_from = (pnt.y - reach) / ACCURACY;
        int32_t y_to   = (pnt.y + reach) / ACCURACY;

        if (x_from < 0) x_from = 0;
        if (y_from < 0) y_from = 0;
        if (x_to > (int32_t)LedMatrix.width() - 1) x_to = (int32_t)LedMatrix.width() - 1;
        if (y_to > (int32_t)LedMatrix.height() - 1) y_to = (int32_t)LedMatrix.height() - 1;

        for (int32_t i = x_from; i <= x_to; ++i) {
            const int32_t dx = i * ACCURACY + ACCURACY / 2 - pnt.x;
            const int32_t dx2 = dx * dx;

            for (int32_t j = y_from; j <= y_to; ++j) {
                const int32_t dy = j * ACCURACY + ACCURACY / 2 - pnt.y;
                const int32_t dist2 = dx2 + dy * dy;
                if (dist2 >= reach2) {
                    continue;
                }

                const uint8_t bright = get_brithtness(sqrt16((uint16_t)dist2));
                if (bright == 0) {
                    continue;
                }

                auto &clr = LedMatrix.at((index_t)i, (index_t)j);
                clr.r = qadd8(clr.r, scale8(pnt.color.r, bright));
                clr.g = qadd8(clr.g, scale8(pnt.color.g, bright));
                clr.b = qadd8(clr.b, scale8(pnt.color.b, bright));
            }
        }
    }

public:
    Points() {}

    void on_init()
    {
        int i;
        tick = 0;
        point_size = 30;
        bright_radius = 40;

        if (bright_radius > MAX_BRIGHT_RADIUS) {
            bright_radius = MAX_BRIGHT_RADIUS;
        }
        for (i = 0; i < bright_radius; ++i) {
            bright_table[i] = (uint8_t)(get_func_brithtness(point_size + i) * 255);
        }

        for (i = 0; i < POINTS_AMNT; ++i) {
            points[i].x = random16(0, LedMatrix.width() * ACCURACY);
            points[i].y = random16(0, LedMatrix.height() * ACCURACY);

            points[i].vec_x = random(0, MAX_VEC_SIZE * 2) - MAX_VEC_SIZE;
            points[i].vec_y = random(0, MAX_VEC_SIZE * 2) - MAX_VEC_SIZE;
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