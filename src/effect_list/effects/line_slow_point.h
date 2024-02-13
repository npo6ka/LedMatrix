#pragma once

#include "effect_list/effect.h"

class SlowPoints : public Effect
{
    struct Point {
        uint16_t pos;
        int16_t bright;
        CRGB color;
    };

    uint8_t step;
    uint8_t tick = 0;

    static const uint8_t point_height = 16;
    static const uint8_t max_point_num = LEDS_CNT / point_height * 2;
    Point points[max_point_num] = {};

    void sort() {
        for (int i = 0; i < max_point_num - 1; ++i) {
            bool swap_flag = false;
            for (int j = max_point_num - 1; j > i; --j) {
                if (points[j - 1].pos > points[j].pos) {
                    Point pnt = points[j - 1];
                    points[j - 1] = points[j];
                    points[j] = pnt;
                    swap_flag = true;
                }
            }
            if (!swap_flag) {
                break;
            }
        }
    }

    void try_gen_new_point() {
        sort();
        uint8_t cnt = 0; // считаем кол-во пустых point'ов
        while(cnt < max_point_num && points[cnt].pos == 0) {
            cnt++;
        }
        // для них пытаемся сгенерировать новые point'ы
        for (int i = 0; i < cnt; ++i) {
            if (random8() < 3) {
                uint16_t max_pos = 0;
                uint16_t min_pos = 0; //ищем максимальное растояние между точками
                for (int j = 1; j < max_point_num; ++j) {
                    if (points[j].pos - points[j - 1].pos > max_pos - min_pos) {
                        min_pos = points[j - 1].pos;
                        max_pos = points[j].pos;
                    }
                }

                // проверяем ещё последний диапозон
                if (LEDS_CNT - 1 - points[max_point_num - 1].pos > max_pos - min_pos) {
                    min_pos = points[max_point_num - 1].pos;
                    max_pos = LEDS_CNT - 1;
                }

                min_pos = random16() % (max_pos - min_pos + 1) + min_pos; // подумать как сделать вероятность близкой к центру

                if (points[0].pos != 0) {
                    out("point is not null\n");
                }
                points[0] = {min_pos, 1, CHSV(tick, 255, 255)};

                sort();
            }
        }
    }


    void draw_point(int16_t pos, CRGB color, uint16_t radius) {
        int16_t start_pos = pos - radius;
        int16_t end_pos = pos + radius;
        CRGB *leds = getLeds();
        for (uint16_t i = 0; i < radius; ++i) {
            if (start_pos + i >= 0 && start_pos + i< LEDS_CNT) {
                leds[start_pos + i] += color.scale8(((int32_t)255 * i / radius));
            }
        }
        for (uint16_t i = radius; i > 0; --i) {
            if (end_pos - i >= 0 && end_pos - i < LEDS_CNT) {
                leds[end_pos - i] += color.scale8(((int32_t)255 * i / radius));
            }
        }
    }


public:
    SlowPoints() {}

    void on_init() {
        set_fps(60);
        step = 2;
    }

    void on_update() {
        for (int i = 0; i < max_point_num; i++) {
            if (points[i].bright > 0) { // bright 0 .. 255
                points[i].bright += step;
                if (points[i].bright >= 255) {
                    points[i].bright = -255;
                }
            } else if (points[i].bright < 0) { // bright -255 .. 0
                points[i].bright += step;
                if (points[i].bright >= 0) {
                    points[i].bright = 0;
                    points[i].pos = 0;
                }
            }
        }

        try_gen_new_point();
        if (random8() < 5) tick++;
    }

    void on_render() {
        FastLED.clear();
        for (int i = 0; i < max_point_num; i++) {
            if (points[i].bright != 0) {
                CRGB color = points[i].color.scale8(abs(points[i].bright));
                draw_point(points[i].pos, color, point_height);
            }
        }
    }
};