#pragma once

#include "effect.h"

class DynamicSquare : public Effect
{
    uint8_t fade_step = 192;
    uint8_t radius = WIDTH > HEIGHT ? (HEIGHT): (WIDTH);
    //uint32_t color = 0x0000ff;
    uint8_t hsv = 0;
    uint8_t cur_ring = 0;

public:
    DynamicSquare() {}

    void on_init() {
        set_fps(12);
    }

    void draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, CRGB color) {
        drawLine(x1, y1, x1, y2, color);
        drawLine(x1, y1, x2, y1, color);
        drawLine(x1, y2, x2, y2, color);
        drawLine(x2, y1, x2, y2, color);
    }

    void on_update() {
        fader(fade_step);

        if (cur_ring <= radius / 2) {
            draw_rectangle(cur_ring, cur_ring, HEIGHT - cur_ring - 1, WIDTH - cur_ring - 1, CHSV(hsv, 255, 255));
        } else {
            draw_rectangle(radius - cur_ring + 1, radius - cur_ring + 1, HEIGHT - (radius - cur_ring + 1) - 1, WIDTH - (radius - cur_ring + 1) - 1, CHSV(hsv, 255, 255));
        }

        cur_ring = (cur_ring + 1) % (radius);
        hsv = (hsv + 1) % 256;
    }
};
