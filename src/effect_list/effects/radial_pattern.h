#pragma once
// Source: https://editor.soulmatelights.com/gallery/1586-radial-pattern
// This variant: https://editor.soulmatelights.com/gallery/2778-radialpattern
#include "effect_list/effect.h"

#define C_X (LEDS_WIDTH / 2)
#define C_Y (LEDS_HEIGHT / 2)

class RadialPattern : public Effect
{
    uint8_t XY_angle[LEDS_WIDTH][LEDS_HEIGHT];
    uint8_t XY_radius[LEDS_WIDTH][LEDS_HEIGHT];
    uint32_t t;

    const byte speed = 1;

public:
    RadialPattern() {}

    void on_init() {
        for (int8_t x = -C_X; x < C_X + (LEDS_WIDTH % 2); x++) {
            for (int8_t y = -C_Y; y < C_Y + (LEDS_HEIGHT % 2); y++) {
                XY_angle[x + C_X][y + C_Y] = 128 * (atan2(y, x) / PI);
                XY_radius[x + C_X][y + C_Y] = hypot(x, y) * 255 / LEDS_WIDTH; //thanks Sutaburosu
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

        blur2d(LedMatrix.leds(), LedMatrix.width(), LedMatrix.height(), 64);
    }
};
