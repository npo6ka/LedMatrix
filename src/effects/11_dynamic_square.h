#pragma once

#include "effect.h"

class DynamicSquare : public Effect
{
    uint8_t lenght = 2;
    uint8_t step = MAX_BRIGHTNESS / lenght;
    uint8_t radius = WIDTH > HEIGHT ? (HEIGHT / 2): (WIDTH / 2);
    //uint32_t color = 0x0000ff;
    uint8_t hsv = 0;

public:
    DynamicSquare() {}

    void on_init() {

    }

    void on_update() {
        static uint8_t cur_ring = 0;

        CRGB color = CHSV(hsv, 255, 255);

        fader(step);
        drawLine(cur_ring, cur_ring, WIDTH - cur_ring - 1, cur_ring, color);
        drawLine(cur_ring, cur_ring, cur_ring, HEIGHT - cur_ring - 1, color);
        drawLine(WIDTH - cur_ring - 1, HEIGHT - cur_ring - 1, cur_ring, HEIGHT - cur_ring - 1, color);
        drawLine(WIDTH - cur_ring - 1, HEIGHT - cur_ring - 1, WIDTH - cur_ring - 1, cur_ring, color);

        cur_ring = (cur_ring + 1) % (radius * 2);
        hsv = (hsv + 1) % 256;
        //delay(25);
    }
};
