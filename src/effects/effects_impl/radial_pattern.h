#pragma once
// Source: https://editor.soulmatelights.com/gallery/1586-radial-pattern
// This variant: https://editor.soulmatelights.com/gallery/2778-radialpattern
#include "effects/effect.h"

class RadialPattern : public Effect
{
    uint8_t XY_angle[LEDS_WIDTH][LEDS_HEIGHT];
    uint8_t XY_radius[LEDS_WIDTH][LEDS_HEIGHT];
    uint32_t t;

    const byte speed = 1;

public:
    RadialPattern() {}

    void on_init() {
        const int w = (int)LedMatrix.width();
        const int h = (int)LedMatrix.height();
        const int cx = w / 2;
        const int cy = h / 2;
        const int norm = max(w, 1);
        for (int16_t x = -cx; x < cx + (w % 2); x++) {
            for (int16_t y = -cy; y < cy + (h % 2); y++) {
                XY_angle[x + cx][y + cy] = 128 * (atan2(y, x) / PI);
                int r = (int)(hypot(x, y) * 255 / norm);
                if (r > 255) r = 255;
                XY_radius[x + cx][y + cy] = (uint8_t)r; //thanks Sutaburosu
            }
        }
        set_fps(60);
    }

    void on_update() {
        FastLED.clear();
        t += speed;

        for (auto x : LedMatrix.rangeX()) {
            for(auto y : LedMatrix.rangeY()) {
                uint8_t angle = XY_angle[x][y];
                uint8_t radius = XY_radius[x][y];
                //uint8_t cl1 = (((angle+t/6)/32%2) ^ (((radius-t)/64)%2))*220;
                uint8_t cl1 = (((angle+t)/32%2) ^ (((radius-t/2)/64)%2))*255;
                //uint8_t cl1 = (((angle + t)/32%2) ^ (((radius)/64)%2))*255;

                if (cl1) {
                    LedMatrix.at(x, y) = CHSV(t / 10, 255, 255);
                }
            }
        }

        LedMatrix.blur(64);
    }
};
