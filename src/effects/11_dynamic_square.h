#pragma once

#include "effect.h"

class DynamicSquare : public Effect
{
    uint8_t fade_step = 192;
    uint8_t radius = WIDTH > HEIGHT ? (HEIGHT / 2): (WIDTH / 2);
    //uint32_t color = 0x0000ff;
    uint8_t hsv = 0;
    uint8_t cur_ring = 0;

public:
    DynamicSquare() {}

    void on_init() {
        set_fps(10);
    }

    void on_update() {
        CRGB color = CHSV(hsv, 255, 255);

        fader(fade_step);
        drawLine(cur_ring, cur_ring, HEIGHT - cur_ring - 1, cur_ring, color);
        drawLine(cur_ring, cur_ring, cur_ring, WIDTH - cur_ring - 1, color);
        drawLine(HEIGHT - cur_ring - 1, WIDTH - cur_ring - 1, cur_ring, WIDTH - cur_ring - 1, color);
        drawLine(HEIGHT - cur_ring - 1, WIDTH - cur_ring - 1, HEIGHT - cur_ring - 1, cur_ring, color);

        cur_ring = (cur_ring + 1) % (radius);
        hsv = (hsv + 1) % 256;
        //delay(25);
    }
};
