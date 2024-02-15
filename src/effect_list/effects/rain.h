#pragma once

#include "effect_list/effect.h"

class Rain : public Effect
{
    uint8_t step = 2;

public:
    Rain() {}

    void on_init() {
        set_fps(60);
    }

    void on_update() {
        int i;

        for (i = 0; i < LEDS_CNT; ++i) {
            CRGB &cl = getLeds()[i];

            if (random8(255) == 0) {
                cl = CRGB(0, 0, 255);
            } else if (cl.b > 0) {
                if (cl.b > step) {
                    cl = CRGB(0, 0, cl.b - step);
                } else {
                    cl = CRGB(0, 0, 0);
                }
            }
        }
    }
};
