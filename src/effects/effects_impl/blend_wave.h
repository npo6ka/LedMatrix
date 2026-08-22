#pragma once

#include "effects/effect.h"

// Две плавные волны, которые смешиваются и едут вдоль ленты.
class BlendWave : public Effect {
    // Оттенки считаются в int без приведения к кругу: линейная интерполяция
    // остаётся непрерывной, а перенос через 255 даёт обычный оборот палитры.
    static void fillHueGradient(int startPos, int startHue, int endPos, int endHue) {
        const int dist = endPos - startPos;
        const int divisor = dist ? dist : 1;
        for (int i = startPos; i <= endPos; ++i) {
            const int hue = startHue + ((endHue - startHue) * (i - startPos)) / divisor;
            LedMatrix.atLinear(i) = CHSV((uint8_t)hue, 255, 255);
        }
    }

public:
    void on_init() override {
        set_fps(60);
    }

    void on_update() override {
        const int n = (int)LedMatrix.size();
        if (n < 10) {
            return;
        }

        const int hue = beatsin8(3, 0, 255);
        // Разбег не больше половины круга — иначе направление обхода
        // оттенков переворачивается и лента скачком меняет цвет.
        const int spread = beatsin8(4, 0, 128);
        const int loc = (int)beatsin16(10, 0, n - 1);

        fillHueGradient(0, hue, loc, hue + spread);
        fillHueGradient(loc, hue + spread, n - 1, hue);
    }
};
