#pragma once

#include "effects/effect.h"

// Несколько цветных точек, которые качаются по ленте вразнобой и оставляют шлейф.
// Порт juggle_pal (Mark Kriegsman / Andrew Tuline / ChristmasLights).
class Juggle : public Effect {
    static constexpr uint8_t kDots = 4;
    static constexpr uint8_t kFade = 16;
    static constexpr uint8_t kBeat = 8;
    static constexpr uint8_t kHueStep = 32;
    static constexpr uint8_t kHueBounceMax = 16;

    int _prev[kDots] = {};
    int8_t _dir[kDots] = {};
    uint8_t _hue[kDots] = {};
    bool _hasPrev = false;

    static uint8_t bounceHueDelta() {
        uint8_t d = 1 + random8(kHueBounceMax);
        return random8(2) ? d : (uint8_t)(256 - d);
    }

public:
    void on_init() override {
        set_fps(60);
        _hasPrev = false;
        for (uint8_t i = 0; i < kDots; ++i) {
            _dir[i] = 0;
            _hue[i] = (uint8_t)(i * kHueStep);
        }
    }

    void on_update() override {
        const int n = (int)LedMatrix.size();
        if (n < 10) {
            return;
        }

        LedMatrix.fader(kFade);

        for (uint8_t i = 0; i < kDots; ++i) {
            const int pos = (int)beatsin16((uint8_t)(kBeat + i + kDots), 0, n - 1);
            const int from = _hasPrev ? _prev[i] : pos;

            if (_hasPrev && pos != from) {
                const int8_t dir = pos > from ? 1 : -1;
                if (_dir[i] != 0 && dir != _dir[i]) {
                    _hue[i] += bounceHueDelta();
                }
                _dir[i] = dir;
            }

            const CRGB color = ColorFromPalette(RainbowColors_p, _hue[i], 255, LINEARBLEND);
            const int step = pos >= from ? 1 : -1;
            for (int p = from; p != pos + step; p += step) {
                LedMatrix.atLinear(p) += color;
            }

            _prev[i] = pos;
        }
        _hasPrev = true;
    }
};
