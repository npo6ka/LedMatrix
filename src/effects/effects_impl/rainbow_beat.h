#pragma once

#include "effects/effect.h"

// Пульсирующая радуга вдоль ленты: оттенок качается двумя beatsin.
class RainbowBeat : public Effect {
public:
    void on_init() override {
        set_fps(60);
    }

    void on_update() override {
        const int n = (int)LedMatrix.size();
        if (n < 10) {
            return;
        }

        const uint8_t hue = (beatsin8(17, 0, 255) + beatsin8(13, 0, 255)) / 2;
        for (int i = 0; i < n; ++i) {
            LedMatrix.atLinear(i) = CHSV(hue + i * 8, 255, 255);
        }
    }
};
