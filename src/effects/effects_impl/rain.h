#pragma once

#include "effects/effect.h"
class Rain : public Effect
{
    uint8_t step{2};

public:
    Rain() {}

    void on_init() {
        set_fps(60);
    }

    void on_update() {
        for (size_t i = 0; i < LedMatrix.size(); ++i) {
            auto& led = LedMatrix.at(i);

            if (random8(255) == 0) {
                led = CRGB(0, 0, 255);
            } else if (led.b > 0) {
                if (led.b > step) {
                    led = CRGB(0, 0, led.b - step);
                } else {
                    led = CRGB(0, 0, 0);
                }
            }
        }
    }
};
