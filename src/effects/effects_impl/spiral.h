#pragma once

#include "effects/effect.h"

// Вращающаяся спираль из центра с плавной радужной сменой цвета
class Spiral : public Effect
{
    static constexpr float angleStep = 0.12f;   // скорость вращения
    static constexpr float radiusStep = 0.4f;   // шаг радиуса на виток (плотность спирали)
    static constexpr uint8_t hueStep = 8;      // шаг оттенка по длине спирали

    float _angle = 0;
    uint8_t _hueOffset = 0;

public:
    Spiral() = default;

    void on_init() override {
        set_fps(40);
        _angle = 0;
        _hueOffset = random8(255);
    }

    void on_update() override {
        LedMatrix.fader(35);

        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();
        const float cx = (w - 1) / 2.0f;
        const float cy = (h - 1) / 2.0f;
        const float maxR = sqrtf(cx * cx + cy * cy) + 1.0f;

        // Рисуем точки спирали: r от 0 до maxR, угол = _angle + r * k
        for (float r = 0.5f; r < maxR; r += 0.45f) {
            float a = _angle + r * radiusStep;
            float x = cx + cosf(a) * r;
            float y = cy + sinf(a) * r;
            index_t ix = (index_t)(x + 0.5f);
            index_t iy = (index_t)(y + 0.5f);

            if (ix < w && iy < h) {
                uint8_t hue = (uint8_t)(_hueOffset + (uint8_t)(r * hueStep));
                LedMatrix.at(ix, iy) = CHSV(hue, 255, 255);
            }
        }

        _angle += angleStep;
        _hueOffset += 2;
    }
};
