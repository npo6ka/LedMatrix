#pragma once

#include "effects/effect.h"

// Две встречные волны по радужной палитре.
class TwoWaves : public Effect {
    static constexpr uint8_t kFreq = 2;
    static constexpr int8_t kSpeed1 = 1;
    static constexpr int8_t kSpeed2 = 2;
    static constexpr int8_t kDir = 1;
    static constexpr uint8_t kRot1 = 1;
    static constexpr uint8_t kRot2 = 1;
    static constexpr uint8_t kCutoff1 = 64;
    static constexpr uint8_t kCutoff2 = 96;

    uint8_t _hue1 = 0;
    uint8_t _hue2 = 128;
    int _phase1 = 0;
    int _phase2 = 0;

    // Ниже cutoff — темно, выше — яркость растягивается до 255.
    static uint8_t waveBright(uint8_t wave, uint8_t cutoff) {
        if (wave <= cutoff) {
            return 0;
        }
        return (uint8_t)(((uint16_t)(wave - cutoff) * 255) / (255 - cutoff));
    }

public:
    void on_init() override {
        set_fps(60);
        _hue1 = 0;
        _hue2 = 128;
        _phase1 = 0;
        _phase2 = 0;
    }

    void on_update() override {
        const int n = (int)LedMatrix.size();
        if (n < 10) {
            return;
        }

        _phase1 += kSpeed1 * kDir;
        _phase2 += kSpeed2 * kDir;
        _hue1 += kRot1;
        _hue2 += kRot2;

        for (int k = 0; k < n; ++k) {
            const uint8_t b1 = waveBright(cubicwave8((uint8_t)(k * kFreq + _phase1)), kCutoff1);
            const uint8_t b2 = waveBright(cubicwave8((uint8_t)(k * kFreq + 128 + _phase2)), kCutoff2);
            CRGB c = ColorFromPalette(RainbowColors_p, _hue1, b1, LINEARBLEND);
            c += ColorFromPalette(RainbowColors_p, _hue2, b2, LINEARBLEND);
            LedMatrix.atLinear(k) = c;
        }
    }
};
