#pragma once

#include "effect.h"

class Confetti : public Effect
{
    uint8_t brightness_step = 30;
    uint8_t density = 3;
public:
    Confetti() {}

    void on_init() {

    }

    void on_update() {
        for (uint8_t i = 0; i < density; i++) {
            uint8_t x = random(WIDTH);
            uint8_t y = random(HEIGHT);
            if (!getPixColor(x, y)) {
                setPixColor(x, y, CHSV(random(0, 255), 255, 255));
            }
            fader(brightness_step);
        }
    }
};
