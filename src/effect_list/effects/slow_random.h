#pragma once

#include "effect_list/effect.h"

class SlowRandom : public Effect
{
    uint8_t inc_val[LEDS_SIZE];
    int step;

public:
    SlowRandom() {}

    uint8_t gen_led(uint8_t &color_val)
    {
        uint8_t val = random8(2);

        if (val > 0) {
            color_val = random8();
        }

        return val;
    }

    void on_init() {
        step = 1;
        set_fps(120);

        for (size_t i = 0; i < LedMatrix.size(); i++) {
            auto &led = LedMatrix.at(i);
            inc_val[i] =
                gen_led(led.r) << 4 |
                gen_led(led.g) << 2 |
                gen_led(led.b);
        }
    }

    void on_update() {
        for (size_t i = 0; i < LedMatrix.size(); i++) {
            auto &led = LedMatrix.at(i);
            inc_val[i] =
                proc_val(led.r, (inc_val[i] >> 4) & 0x3, led) << 4 |
                proc_val(led.g, (inc_val[i] >> 2) & 0x3, led) << 2 |
                proc_val(led.b, (inc_val[i] >> 0) & 0x3, led);
        }
    }

    uint8_t proc_val(uint8_t &color_val, uint8_t val, const CRGB &full_color) {
        if (color_val == 0 || color_val == 255) {
            val = 0;
        }
        /*if (val == 0 && random8(100) == 0) {
            val = color_val == 0 ? 1 : 2;
        }*/

        if (val == 0) {
            uint8_t filled_colors = ((uint16_t)full_color.r + full_color.g + full_color.b) / 3;
            if (color_val == 255) {
                // filled_colors [85 (min_chance/256%) .. 255 (max_chance/256%)]
                uint8_t min_chance = 1, max_chance = 10;
                uint8_t chance = (filled_colors - 85) * 1.5; // 0 .. 255
                chance = min_chance + (chance * (max_chance - min_chance) / 255); // 1 .. 10

                if (random8() < chance) {
                    val = 2;
                }
            } else if (color_val == 0) {
                // filled_colors [0(max_chance/256%) .. 170(min_chance/256%)]
                uint8_t min_chance = 1, max_chance = 1;
                uint8_t chance = 255 - (filled_colors * 1.5); // 0 .. 255
                chance = min_chance + (chance * (max_chance - min_chance) / 255); // 1 .. 10

                if (random8() < chance) {
                    val = 1;
                }
            }
        }
        if (val == 2) {
            if (color_val < step) {
                color_val = 0;
            } else {
                color_val = (color_val - step) % 256;
            }
        } else if (val == 1) {
            if (color_val + step > 255) {
                color_val = 255;
            } else {
                color_val = (color_val + step) % 256;
            }
        }

        return val & 0x3;
    }
};
