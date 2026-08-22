#pragma once

#include "effects/effect.h"

// 8-bit шум Перлина вдоль ленты, цвет из радужной палитры.
// Порт noise8_pal (Mark Kriegsman / ChristmasLights).
class Noise8 : public Effect {
    static constexpr uint8_t kScale = 8;
    static constexpr uint32_t kDirFlipMs = 20000;

    uint16_t _dist = 12345;
    int8_t _dir = 1;
    uint32_t _lastFlip = 0;

public:
    void on_init() override {
        set_fps(60);
        _dist = 12345;
        _dir = 1;
        _lastFlip = millis();
    }

    void on_update() override {
        const int n = (int)LedMatrix.size();
        if (n < 10) {
            return;
        }

        if (millis() - _lastFlip >= kDirFlipMs) {
            _dir = (int8_t)(-_dir);
            _lastFlip = millis();
        }

        for (int i = 0; i < n; ++i) {
            const uint16_t x = (uint16_t)(i * kScale);
            const uint8_t index = inoise8(x, _dist + x);
            LedMatrix.atLinear(i) = ColorFromPalette(RainbowColors_p, index, 255, LINEARBLEND);
        }

        _dist += (uint16_t)(beatsin8(10, 1, 4) * _dir);
    }
};
