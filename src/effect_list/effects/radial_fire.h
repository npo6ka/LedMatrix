#pragma once
// Source: https://editor.soulmatelights.com/gallery/1570-radialfire
// This variant: https://editor.soulmatelights.com/gallery/2777-radial-fire
#include "effect_list/effect.h"

#define C_X (LEDS_WIDTH / 2)
#define C_Y (LEDS_HEIGHT / 2)

class RadialFire : public Effect
{
    uint32_t t;
    uint8_t XY_angle[LEDS_WIDTH][LEDS_HEIGHT];
    uint8_t XY_radius[LEDS_WIDTH][LEDS_HEIGHT];

    const uint8_t scaleX = 16;
    const uint8_t scaleY = 1;
    const uint8_t speed = 24;

public:
    RadialFire() {}

    void on_init() {
        float coef = 1 - (LedMatrix.height() > LedMatrix.width() ? 0 : float(abs(LedMatrix.width() - LedMatrix.height())) / max(LedMatrix.width(), LedMatrix.height()) * 0.5);
        for (int8_t x = -C_X; x < C_X + (LedMatrix.width() % 2); x++) {
            for (int8_t y = -C_Y; y < C_Y + (LedMatrix.height() % 2); y++) {
                XY_angle[x + C_X][y + C_Y] = 128 * (atan2(y, x) / PI);
                XY_radius[x + C_X][y + C_Y] = hypot(x, y) * coef * 1.5; //thanks Sutaburosu
            }
        }
        set_fps(30);
    }

    void on_update() {
        t += speed;

        for (auto x : LedMatrix.rangeX()) {
            for(auto y : LedMatrix.rangeY()) {
                uint8_t angle = XY_angle[x][y];
                uint8_t radius = XY_radius[x][y];
                int16_t Bri = inoise8(angle*scaleX, (radius * scaleY)-t ) - radius * (255 / LedMatrix.width());
                uint8_t Col = Bri;
                if (Bri < 0) Bri = 0;
                if (Bri != 0) Bri = 256 - (Bri * 0.2);
                nblend(LedMatrix.at(x, y), ColorFromPalette(HeatColors_p, Col, Bri), speed);
            }
        }
    }
};
