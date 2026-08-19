#pragma once

#include "effects/effect.h"

class MusicSpectrum : public Effect {
    float _smooth[16] = {};

    static uint8_t bandHeight(float value, index_t maxHeight) {
        if (value < 0.0f) {
            value = 0.0f;
        }
        const float scaled = value / 1200.0f;
        if (scaled > 1.0f) {
            return static_cast<uint8_t>(maxHeight);
        }
        return static_cast<uint8_t>(scaled * static_cast<float>(maxHeight));
    }

public:
    static constexpr InputCapability kRequired = InputCapability::Audio;

    void on_init() override {
        set_fps(30);
        for (uint8_t i = 0; i < 16; ++i) {
            _smooth[i] = 0.0f;
        }
    }

    void on_update(const InputSnapshot &input) override {
        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();
        const uint16_t bands = input.audio.available ? input.audio.bandCount : 0;

        LedMatrix.clear();

        for (index_t x = 0; x < w; ++x) {
            float level = 0.0f;
            if (input.audio.available && x < bands && input.audio.bands) {
                level = input.audio.bands[x];
            }

            _smooth[x] = _smooth[x] * 0.65f + level * 0.35f;
            const uint8_t barHeight = bandHeight(_smooth[x], h);

            for (index_t y = 0; y < barHeight && y < h; ++y) {
                const index_t row = static_cast<index_t>(h - 1 - y);
                LedMatrix.at(x, row) = CHSV(static_cast<uint8_t>(x * 16), 255, 255);
            }
        }
    }

    void on_update() override {}
};
