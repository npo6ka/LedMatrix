#pragma once

#include "effect.h"

class ErrorEffect : public Effect
{
public:
    ErrorEffect(const std::string& variableStoragePath) : Effect("") {}

    void on_update() {
        FastLED.showColor(CRGB::Red);
        logError("ERROR state");
    }
};
