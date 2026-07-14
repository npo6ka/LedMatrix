#pragma once

#include "effects/effect.h"

class AllRandom : public Effect
{

public:
    AllRandom() {}

    void on_init() {
        set_fps(15);
    }

    void on_update() {
        for (size_t i = 0; i < LedMatrix.size(); ++i) {
            LedMatrix.at(i) = CRGB(random8(255), random8(255), random8(255));
        }
    }
};
