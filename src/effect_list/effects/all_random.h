#pragma once

#include "effect_list/effect.h"

class AllRandom : public Effect
{

public:
    AllRandom() {}

    void on_init() {
        set_fps(15);
    }

    void on_update() {
        for (auto& led : LedMatrix) {
            led = CRGB(random8(255), random8(255), random8(255));
        }
    }
};
