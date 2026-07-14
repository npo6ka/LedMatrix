#pragma once

#include "effects/effect.h"

// Расходящиеся от центра кольца, которые затухают
class PulseRings : public Effect
{
    static constexpr uint8_t maxRings = 3;
    static constexpr float expandSpeed = 0.35f;
    static constexpr float fadeStart = 3.0f;  // радиус, с которого начинается затухание

    struct Ring {
        float radius = 0;
        uint8_t hue = 0;
        bool active = false;
    };
    Ring _rings[maxRings];
    uint8_t _hueOffset = 0;

    static float dist(float cx, float cy, index_t x, index_t y) {
        float dx = (float)x - cx;
        float dy = (float)y - cy;
        return sqrtf(dx * dx + dy * dy);
    }

public:
    PulseRings() = default;

    void on_init() override {
        set_fps(35);
        _hueOffset = random8(255);
        for (uint8_t i = 0; i < maxRings; i++) {
            _rings[i].active = false;
            _rings[i].radius = 0;
        }
    }

    void on_update() override {
        LedMatrix.fader(25);

        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();
        const float cx = (w - 1) / 2.0f;
        const float cy = (h - 1) / 2.0f;
        const float maxR = dist(cx, cy, 0, 0) + 2.0f;

        // Спавн нового кольца, если есть свободный слот
        for (uint8_t i = 0; i < maxRings; i++) {
            if (!_rings[i].active) {
                _rings[i].active = true;
                _rings[i].radius = 0.5f;
                _rings[i].hue = _hueOffset;
                _hueOffset += 30;
                break;
            }
        }

        for (index_t y = 0; y < h; y++) {
            for (index_t x = 0; x < w; x++) {
                float d = dist(cx, cy, x, y);

                for (uint8_t i = 0; i < maxRings; i++) {
                    if (!_rings[i].active) continue;

                    float r = _rings[i].radius;
                    float diff = fabsf(d - r);
                    if (diff < 0.8f) {
                        uint8_t bright = 200;
                        if (r > fadeStart) {
                            bright = (uint8_t)(bright * (maxR - r) / (maxR - fadeStart));
                        }
                        auto& pix = LedMatrix.at(x, y);
                        CRGB add = CHSV(_rings[i].hue, 255, bright);
                        pix.r = qadd8(pix.r, add.r);
                        pix.g = qadd8(pix.g, add.g);
                        pix.b = qadd8(pix.b, add.b);
                    }
                }
            }
        }

        for (uint8_t i = 0; i < maxRings; i++) {
            if (!_rings[i].active) continue;
            _rings[i].radius += expandSpeed;
            if (_rings[i].radius >= maxR) {
                _rings[i].active = false;
            }
        }
    }
};
