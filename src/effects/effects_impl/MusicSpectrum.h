#pragma once

#include "effects/effect.h"

class MusicSpectrum : public Effect {
    static constexpr uint8_t kMaxDisplayBands = 8;

    float _smooth[kMaxDisplayBands] = {};
    float _peakDot[kMaxDisplayBands] = {};
    float _bandPeak[kMaxDisplayBands] = {};
    float _peakHold = 0.0f;
    float _noiseFloor = 0.0f;

    static constexpr float kDotFallSpeed = 0.22f;
    static constexpr float kAgcHeadroom = 0.55f;
    static constexpr float kDisplayGain = 1.5f;
    static constexpr float kLocalMix = 0.82f;      // per-band AGC — выравнивает полосы
    static constexpr float kLevelGamma = 0.80f;

    // Исходные полосы FFT (0..15) → 8 столбцов: max(0-2), 3, 4, 5, 6, 7, max(8-9), max(10-15)
    static void srcRange(uint8_t index, uint8_t &from, uint8_t &to) {
        constexpr uint8_t kFrom[] = {0, 3, 4, 5, 6, 7, 8, 10};
        constexpr uint8_t kTo[]   = {2, 3, 4, 5, 6, 7, 9, 15};
        from = kFrom[index];
        to = kTo[index];
    }

    static float sourceBandMax(const float *bands, uint16_t bandCount, uint8_t from, uint8_t to) {
        float peak = 0.0f;
        if (!bands || bandCount == 0) {
            return 0.0f;
        }
        const uint16_t last = (static_cast<uint16_t>(to) < bandCount)
            ? static_cast<uint16_t>(to)
            : static_cast<uint16_t>(bandCount - 1);
        for (uint16_t i = from; i <= last; ++i) {
            if (bands[i] > peak) {
                peak = bands[i];
            }
        }
        return peak;
    }

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

    static uint8_t bandForPos(index_t pos, index_t length, uint8_t bandCount) {
        if (length == 0 || bandCount == 0) {
            return 0;
        }
        const uint8_t band = static_cast<uint8_t>((static_cast<uint32_t>(pos) * bandCount) / length);
        return (band < bandCount) ? band : static_cast<uint8_t>(bandCount - 1);
    }

    void paintPixel(index_t pos, bool horizontal, CRGB color) {
        if (horizontal) {
            LedMatrix.at(pos, 0) = color;
        } else {
            LedMatrix.at(0, pos) = color;
        }
    }

    void paintStrip(index_t length, bool horizontal, uint8_t bandCount) {
        if (length == 0 || bandCount == 0) {
            return;
        }
        for (index_t p = 0; p < length; ++p) {
            const uint8_t band = bandForPos(p, length, bandCount);
            const uint8_t bri = static_cast<uint8_t>(_smooth[band] * 255.0f + 0.5f);
            paintPixel(p, horizontal, CHSV(static_cast<uint8_t>(band * 32), 255, bri));
        }
    }

    void paintMatrix(index_t w, index_t h, uint8_t bandCount) {
        if (w == 0 || h == 0 || bandCount == 0) {
            return;
        }
        for (index_t x = 0; x < w; ++x) {
            const uint8_t band = bandForPos(x, w, bandCount);
            const uint8_t barHeight = static_cast<uint8_t>(_smooth[band] * static_cast<float>(h) + 0.5f);

            for (index_t y = 0; y < barHeight && y < h; ++y) {
                const index_t row = static_cast<index_t>(h - 1 - y);
                LedMatrix.at(x, row) = CHSV(static_cast<uint8_t>(band * 32), 255, 255);
            }

            const index_t peakRow = static_cast<index_t>(_peakDot[band] + 0.5f);
            if (peakRow < h) {
                LedMatrix.at(x, peakRow) = CHSV(static_cast<uint8_t>(band * 32), 40, 255);
            }
        }
    }

public:
    static constexpr InputCapability kRequired = InputCapability::Audio;

    void on_init() override {
        set_fps(30);
        _peakHold = 0.0f;
        _noiseFloor = 0.0f;
        const float startDot = static_cast<float>(LedMatrix.height() > 1 ? LedMatrix.height() : 0);
        for (uint8_t i = 0; i < kMaxDisplayBands; ++i) {
            _smooth[i] = 0.0f;
            _peakDot[i] = startDot;
            _bandPeak[i] = 1.0f;
        }
    }

    void on_update(const InputSnapshot &input) override {
        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();
        const uint16_t rawBands = input.audio.available ? input.audio.bandCount : 0;

        float framePeak = 0.0f;
        if (input.audio.available && input.audio.bands) {
            for (uint16_t i = 0; i < rawBands; ++i) {
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

        for (uint8_t i = 0; i < kMaxDisplayBands; ++i) {
            float level = 0.0f;
            uint8_t srcFrom = 0;
            uint8_t srcTo = 0;
            srcRange(i, srcFrom, srcTo);
            if (hasSignal && input.audio.available && input.audio.bands && rawBands > srcFrom) {
                const float raw = sourceBandMax(input.audio.bands, rawBands, srcFrom, srcTo) - _noiseFloor;
                if (raw > 0.0f) {
                    updateBandPeak(i, raw);
                    level = bandLevel(raw, i, invRange);
                }
            }
            _smooth[i] = smoothBar(_smooth[i], hasSignal ? level : 0.0f);

            if (h > 1) {
                const uint8_t barHeight = static_cast<uint8_t>(_smooth[i] * static_cast<float>(h) + 0.5f);
                const float barTop = static_cast<float>(h - barHeight);
                if (barHeight > 0 && barTop < _peakDot[i]) {
                    _peakDot[i] = barTop;
                }
                _peakDot[i] += kDotFallSpeed;
                const float maxRow = static_cast<float>(h - 1);
                if (_peakDot[i] > maxRow) {
                    _peakDot[i] = maxRow;
                }
            }
        }

        LedMatrix.clear();

        const uint8_t drawBands = kMaxDisplayBands;
        if (h <= 1) {
            paintStrip(w, true, drawBands);
        } else if (w <= 1) {
            paintStrip(h, false, drawBands);
        } else {
            paintMatrix(w, h, drawBands);
        }
    }

    void on_update() override {}
};
