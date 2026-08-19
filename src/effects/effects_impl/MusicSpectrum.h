#pragma once

#include "effects/effect.h"

class MusicSpectrum : public Effect {
    float _smooth[16] = {};
    float _peakDot[16] = {};
    float _bandPeak[16] = {};
    float _peakHold = 0.0f;
    float _noiseFloor = 0.0f;

    static constexpr float kDotFallSpeed = 0.22f;
    static constexpr float kAgcHeadroom = 0.55f;
    static constexpr float kDisplayGain = 1.5f;
    static constexpr float kLocalMix = 0.82f;      // per-band AGC — выравнивает полосы
    static constexpr float kLevelGamma = 0.80f;

    static float clamp01(float value) {
        if (value < 0.0f) {
            return 0.0f;
        }
        if (value > 1.0f) {
            return 1.0f;
        }
        return value;
    }

    static float smoothBar(float current, float target) {
        target = clamp01(target);
        const float diff = target - current;

        constexpr float kMaxRise = 0.12f;
        constexpr float kMaxFall = 0.10f;
        if (diff > kMaxRise) {
            return current + kMaxRise;
        }
        if (diff < -kMaxFall) {
            return current - kMaxFall;
        }
        if (diff > 0.0f) {
            return current * 0.80f + target * 0.20f;
        }
        return current * 0.55f + target * 0.45f;
    }

    void updateBandPeak(uint8_t index, float raw) {
        if (raw > _bandPeak[index]) {
            _bandPeak[index] = _bandPeak[index] * 0.88f + raw * 0.12f;
        } else {
            _bandPeak[index] = _bandPeak[index] * 0.965f + raw * 0.035f;
        }
        if (_bandPeak[index] < 1.0f) {
            _bandPeak[index] = 1.0f;
        }
    }

    float bandLevel(float raw, uint8_t index, float invRange) const {
        const float bandRange = _bandPeak[index] * kAgcHeadroom;
        const float localLevel = raw / bandRange;
        const float globalLevel = raw * invRange;
        const float mixed = localLevel * kLocalMix + globalLevel * (1.0f - kLocalMix);
        float level = clamp01(mixed * kDisplayGain);
        if (level > 0.0f) {
            level = powf(level, kLevelGamma);
        }
        return level;
    }

public:
    static constexpr InputCapability kRequired = InputCapability::Audio;

    void on_init() override {
        set_fps(30);
        _peakHold = 0.0f;
        _noiseFloor = 0.0f;
        for (uint8_t i = 0; i < 16; ++i) {
            _smooth[i] = 0.0f;
            _peakDot[i] = static_cast<float>(LEDS_HEIGHT);
            _bandPeak[i] = 1.0f;
        }
    }

    void on_update(const InputSnapshot &input) override {
        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();
        const uint16_t bands = input.audio.available ? input.audio.bandCount : 0;

        float framePeak = 0.0f;
        if (input.audio.available && input.audio.bands) {
            for (uint16_t i = 0; i < bands; ++i) {
                if (input.audio.bands[i] > framePeak) {
                    framePeak = input.audio.bands[i];
                }
            }
        }

        if (framePeak > _peakHold) {
            _peakHold = _peakHold * 0.90f + framePeak * 0.10f;
        } else {
            _peakHold = _peakHold * 0.97f + framePeak * 0.03f;
        }

        if (framePeak < _noiseFloor) {
            _noiseFloor = _noiseFloor * 0.85f + framePeak * 0.15f;
        } else {
            _noiseFloor = _noiseFloor * 0.998f + framePeak * 0.002f;
        }

        const float range = _peakHold - _noiseFloor;
        const float displayRange = range * kAgcHeadroom;
        const float invRange = (displayRange > 1.0f) ? (1.0f / displayRange) : 0.0f;
        const bool hasSignal = range > 1.0f && framePeak > _noiseFloor * 1.15f;

        LedMatrix.clear();

        for (index_t x = 0; x < w; ++x) {
            float level = 0.0f;
            if (hasSignal && input.audio.available && x < bands && input.audio.bands) {
                const float raw = input.audio.bands[x] - _noiseFloor;
                if (raw > 0.0f && x < 16) {
                    updateBandPeak(static_cast<uint8_t>(x), raw);
                    level = bandLevel(raw, static_cast<uint8_t>(x), invRange);
                }
            }

            if (hasSignal) {
                _smooth[x] = smoothBar(_smooth[x], level);
            } else {
                _smooth[x] = smoothBar(_smooth[x], 0.0f);
            }

            const uint8_t barHeight = static_cast<uint8_t>(_smooth[x] * static_cast<float>(h) + 0.5f);

            for (index_t y = 0; y < barHeight && y < h; ++y) {
                const index_t row = static_cast<index_t>(h - 1 - y);
                LedMatrix.at(x, row) = CHSV(static_cast<uint8_t>(x * 16), 255, 255);
            }

            if (x < 16) {
                const float barTop = static_cast<float>(h - barHeight);
                float &dotRow = _peakDot[x];

                if (barHeight > 0 && barTop < dotRow) {
                    dotRow = barTop;
                }
                dotRow += kDotFallSpeed;
                if (dotRow > static_cast<float>(h - 1)) {
                    dotRow = static_cast<float>(h - 1);
                }

                const index_t peakRow = static_cast<index_t>(dotRow + 0.5f);
                if (peakRow < h) {
                    LedMatrix.at(x, peakRow) = CHSV(static_cast<uint8_t>(x * 16), 40, 255);
                }
            }
        }
    }

    void on_update() override {}
};
