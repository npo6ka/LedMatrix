#pragma once

#include "effects/effect.h"

// Вращающаяся спираль из центра с плавной радужной сменой цвета
class Spiral : public Effect
{
    static constexpr float angleStep = 0.24f;   // скорость вращения
    static constexpr float radiusStep = 0.4f;   // шаг радиуса на виток (плотность спирали)
    static constexpr uint8_t hueStep = 8;      // шаг оттенка по длине спирали

    // На ленте спираль разворачивается в бегущую от центра волну: витку на расстоянии d
    // от центра соответствует своя фаза. Шаги в единицах 0..255 под табличный cos8
    static constexpr uint8_t linePhaseStep = 16;    // сдвиг фазы на диод (плотность витков)
    static constexpr uint8_t lineSpeedStep = 20;    // сдвиг фазы за кадр (скорость)

    float _angle = 0;
    uint8_t _hueOffset = 0;
    uint8_t _linePhase = 0;
    bool _line_mode = false;

    // Двумерная спираль на ленте вырождается: точки витков не попадают
    // в единственную строку матрицы и отбрасываются
    static bool isLineMatrix() {
        return LedMatrix.width() <= 1 || LedMatrix.height() <= 1;
    }

    void updateLine() {
        const size_t len = LedMatrix.size();
        const size_t center = len ? (len - 1) / 2 : 0;

        for (size_t i = 0; i < len; ++i) {
            const uint16_t d = (uint16_t)(i > center ? i - center : center - i);
            const uint8_t phase = (uint8_t)(_linePhase - (uint8_t)(d * linePhaseStep));
            const uint8_t hue = (uint8_t)(_hueOffset + (uint8_t)(d * hueStep));
            LedMatrix.atLinear((int)i) = CHSV(hue, 255, cos8(phase));
        }

        _linePhase += lineSpeedStep;
    }

    void updateMatrix() {
        LedMatrix.fader(17);

        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();
        const float cx = (w - 1) / 2.0f;
        const float cy = (h - 1) / 2.0f;
        const float maxR = sqrtf(cx * cx + cy * cy) + 1.0f;

        // Рисуем точки спирали: r от 0 до maxR, угол = _angle + r * k
        // Шаг по радиусу уменьшается с ростом r, иначе дуга между соседними
        // точками становится длиннее пикселя и линия рвётся на краях
        for (float r = 0.5f; r < maxR; r += 0.9f / (1.0f + r * radiusStep)) {
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
    }

public:
    Spiral() = default;

    void on_init() override {
        set_fps(40);
        _angle = 0;
        _hueOffset = random8(255);
        _linePhase = 0;
        _line_mode = isLineMatrix();
    }

    void on_update() override {
        if (_line_mode) {
            updateLine();
        } else {
            updateMatrix();
        }

        _hueOffset += 4;
    }
};
