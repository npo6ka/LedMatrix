#pragma once

#include "effects/effect.h"

class StaticColor : public Effect {
    CRGB _color;

    void fill() {
        LedMatrix.drawRect(0, 0, LedMatrix.width(), LedMatrix.height(), _color);
    }

public:
    void on_init() override {
        set_fps(20);
        _color = CHSV(random8(), 255, 255);
        fill();
    }

    void on_update() override {
        fill();
    }
};
