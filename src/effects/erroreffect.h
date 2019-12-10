#pragma once

#include "effect.h"

class ErrorEffect : public Effect
{
public:
    ErrorEffect() {}

    void on_update() {
        FastLED.showColor(CRGB::Red);
    }
};
