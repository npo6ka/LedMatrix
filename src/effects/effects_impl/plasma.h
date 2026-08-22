#pragma once

#include "effects/effect.h"

// Плазма: две волны разной частоты складываются в переливающиеся пятна.
// Порт plasma (Andrew Tuline / ChristmasLights).
class Plasma : public Effect {
    static constexpr uint8_t kFreq1 = 3;
    static constexpr uint8_t kFreq2 = 5;
    static constexpr uint8_t kPhaseBpm1 = 4;
    static constexpr uint8_t kPhaseBpm2 = 18;
    static constexpr uint8_t kFloorBpm = 7;
    static constexpr uint8_t kFloorMax = 96;

public:
    void on_init() override {
        set_fps(60);
    }

    void on_update() override {
        const int n = (int)LedMatrix.size();
        if (n < 10) {
            return;
        }

        const uint8_t phase1 = beatsin8(kPhaseBpm1);
        const uint8_t phase2 = beatsin8(kPhaseBpm2);
        // Плавающий порог гасит слабые участки, добавляя чёрные провалы между пятнами.
        const uint8_t floorLevel = beatsin8(kFloorBpm, 0, kFloorMax);

        for (int k = 0; k < n; ++k) {
            const uint8_t colorIndex = (uint8_t)(cubicwave8((uint8_t)(k * kFreq1 + phase1)) / 2 +
                                                cos8((uint8_t)(k * kFreq2 + phase2)) / 2);
            const uint8_t bright = colorIndex > floorLevel ? (uint8_t)(colorIndex - floorLevel) : 0;
            LedMatrix.atLinear(k) = ColorFromPalette(RainbowColors_p, colorIndex, bright, LINEARBLEND);
        }
    }
};
