#pragma once

#include "effect.h"

class Rain : public Effect
{
    uint8_t step = 2;

public:
    Rain() {}

    void on_init() {
        set_fps(60);
    }

    void on_update() {
        int i, j;

        for (i = 0; i < HEIGHT; ++i) {
            for (j = 0; j < WIDTH; ++j) {
                CRGB &cl = getPix(i, j);

                if (random8(255) == 0) {
                    cl = CRGB(0, 0, MAX_BRIGHTNESS);
                } else if (cl.b > 0) {
                    if (cl.b > step) {
                        cl = CRGB(0, 0, cl.b - step);
                    } else {
                        cl = CRGB(0, 0, 0);
                    }
                }
            }
        }
    }
};
