#pragma once

#include "effects/effect.h"

// Одна бегущая волна по радужной палитре.
class OneWave : public Effect {
    static constexpr uint8_t kFreq = 4;
    static constexpr int8_t kSpeed = 4;
    static constexpr int8_t kDir = 1;
    static constexpr uint8_t kHueStep = 40;   // разница оттенка соседних полосок
    static constexpr uint8_t kHueDrift = 1;   // сдвиг всех оттенков за кадр
    static constexpr uint8_t kCutoff = 96;

    uint8_t _hue = 0;
    int _phase = 0;

    static uint8_t waveBright(uint8_t wave, uint8_t cutoff) {
        if (wave <= cutoff) {
            return 0;
        }
        return (uint8_t)(((uint16_t)(wave - cutoff) * 255) / (255 - cutoff));
    }

public:
    void on_init() override {
        set_fps(60);
        _hue = 0;
        _phase = 0;
    }

    void on_update() override {
        const int n = (int)LedMatrix.size();
        if (n < 10) {
            return;
        }

        _hue += kHueDrift;
        _phase += kDir * kSpeed;

        for (int k = 0; k < n; ++k) {
            const int pos = k * kFreq + _phase;
            const uint8_t b = waveBright(cubicwave8((uint8_t)pos), kCutoff);
            // Целая часть pos — номер полоски: внутри гребня цвет ровный, у каждой волны свой.
            const uint8_t hue = _hue + (uint8_t)((pos >> 8) * kHueStep);
            LedMatrix.atLinear(k) = ColorFromPalette(RainbowColors_p, hue, b, LINEARBLEND);
        }
    }
};
