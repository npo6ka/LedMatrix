#pragma once

#include "effects/effect.h"

// Мигание диодов разными цветами со шлейфом по ленте.
class ColorBlink : public Effect {
    static constexpr uint16_t kXorig = 0x013;
    static constexpr uint16_t kYorig = 0x021;
    static constexpr uint32_t kResetMs = 5000;
    static constexpr uint8_t kFade = 16;

    uint16_t _x = kXorig;
    uint16_t _y = kYorig;
    uint32_t _lastReset = 0;

public:
    void on_init() override {
        set_fps(60);
        _x = kXorig;
        _y = kYorig;
        _lastReset = millis();
    }

    void on_update() override {
        const size_t n = LedMatrix.size();
        if (n < 10) {
            return;
        }

        if (millis() - _lastReset >= kResetMs) {
            _x = kXorig;
            _y = kYorig;
            _lastReset = millis();
        }

        const uint16_t xn = _x - (_y / 2);
        const uint16_t yn = _y + (xn / 2);
        _x = xn;
        _y = yn;

        const uint8_t palIndex = (sin8(_x) + cos8(_y)) / 2;
        LedMatrix.atLinear(_x % n) = ColorFromPalette(RainbowColors_p, palIndex, 255, LINEARBLEND);
        LedMatrix.fader(kFade);
    }
};
