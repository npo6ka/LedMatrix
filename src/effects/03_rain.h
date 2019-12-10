#pragma once

#include "effect.h"

class Rain : public Effect
{
    uint8_t step = 2;

public:
    Rain() {}

    void on_tick() {
        int i, j;

        for (i = 0; i < WIDTH; ++i) {
            for (j = 0; j < HEIGHT; ++j) {
                CRGB cl = getPix(i, j);

                if (random8(100) == 0) {
                    int asd = 0;
                    cl = CRGB(0, 0, MAX_BRIGHTNESS);
                } else if (cl.b > 0) {
                    if (cl.b > step) {
                        cl = CRGB(0, 0, cl.b - step);
                    } else {
                        cl = CRGB(0, 0, 0);
                    }
                }

                getPix(i, j) = cl;
            }
        }
    }
};
