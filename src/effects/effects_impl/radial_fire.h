#pragma once
// Source: https://editor.soulmatelights.com/gallery/1570-radialfire
// This variant: https://editor.soulmatelights.com/gallery/2777-radial-fire
#include "effects/effect.h"

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
        const int w = (int)LedMatrix.width();
        const int h = (int)LedMatrix.height();
        const int cx = w / 2;
        const int cy = h / 2;
        const int max_side = max(w, h);
        float coef = 1 - (h > w ? 0 : float(abs(w - h)) / max(max_side, 1) * 0.5f);
        for (int16_t x = -cx; x < cx + (w % 2); x++) {
            for (int16_t y = -cy; y < cy + (h % 2); y++) {
                XY_angle[x + cx][y + cy] = 128 * (atan2(y, x) / PI);
                int r = (int)(hypot(x, y) * coef * 1.5f);
                if (r > 255) r = 255;
                XY_radius[x + cx][y + cy] = (uint8_t)r; //thanks Sutaburosu
            }
        }
        set_fps(30);
    }

    void on_update() {
        t += speed;
        const uint8_t falloff = 255 / max(LedMatrix.width(), (index_t)1);

        for (auto x : LedMatrix.rangeX()) {
            for(auto y : LedMatrix.rangeY()) {
                uint8_t angle = XY_angle[x][y];
                uint8_t radius = XY_radius[x][y];
                int16_t Bri = inoise8(angle*scaleX, (radius * scaleY)-t ) - radius * falloff;
                uint8_t Col = Bri;
                if (Bri < 0) Bri = 0;
                if (Bri != 0) Bri = 256 - (Bri * 0.2);
                nblend(LedMatrix.at(x, y), ColorFromPalette(HeatColors_p, Col, Bri), speed);
            }
        }
    }
};
