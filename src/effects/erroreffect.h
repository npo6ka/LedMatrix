#pragma once

#include "effect.h"

class ErrorEffect : public Effect
{
public:
    ErrorEffect() {}

    void on_update() {
        for (size_t i = 0; i < LedMatrix.size(); ++i) {
            LedMatrix.at(i) = CRGB::Red;
        }
        logError("ERROR state");
    }
};
