#pragma once

#include "effects/effect.h"

// Две плавные волны, которые смешиваются и едут вдоль ленты.
class BlendWave : public Effect {
    static void fillLinearGradient(int startPos, CRGB startColor, int endPos, CRGB endColor) {
        if (endPos < startPos) {
            const int tmpPos = endPos;
            const CRGB tmpColor = endColor;
            endPos = startPos;
            endColor = startColor;
            startPos = tmpPos;
            startColor = tmpColor;
        }

        const int dist = endPos - startPos;
        const int divisor = dist ? dist : 1;
        for (int i = startPos; i <= endPos; ++i) {
            const uint8_t t = (uint8_t)(((i - startPos) * 255) / divisor);
            LedMatrix.atLinear(i) = blend(startColor, endColor, t);
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

        const uint8_t speed = beatsin8(6, 0, 255);
        const CRGB clr1 = blend(
            CHSV(beatsin8(3, 0, 255), 255, 255),
            CHSV(beatsin8(4, 0, 255), 255, 255),
            speed);
        const CRGB clr2 = blend(
            CHSV(beatsin8(4, 0, 255), 255, 255),
            CHSV(beatsin8(3, 0, 255), 255, 255),
            speed);
        const int loc1 = (int)beatsin16(10, 0, n - 1);

        fillLinearGradient(0, clr2, loc1, clr1);
        fillLinearGradient(loc1, clr2, n - 1, clr1);
    }
};
