#pragma once

#include "Effects/effect.h"

#define PACMAN_W 10
#define PACMAN_H 10

static const uint32_t pacman1[PACMAN_W*PACMAN_H] PROGMEM = {
    0x000000, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0xffff00, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0xffff00, 0xffff00, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
};

static const uint32_t pacman2[PACMAN_W*PACMAN_H] PROGMEM = {
    0x000000, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0xffff00, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0xffff00, 0xffff00, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
};

class Pacman : public Effect
{
public:
    Pacman() {}

    void on_init()
    {
        phase = 0;
        set_fps(5);
    }

    void on_update()
    {
        fader(255);
        CRGB c(255, 255, 255);
        for (int i = PACMAN_W - phase ; i < WIDTH ; i += 4)
        {
            setPixColor(i, PACMAN_H / 2 - 1, c);
            setPixColor(i + 1, PACMAN_H / 2 - 1, c);
            setPixColor(i, PACMAN_H / 2, c);
            setPixColor(i + 1, PACMAN_H / 2, c);
        }
        drawSprite(0, 0, PACMAN_W, PACMAN_H, phase / 2 ? pacman1 : pacman2);
        phase = (phase + 1) % 4;
    }
private:
    static void drawSprite(int x, int y, int w, int h, const uint32_t *spr)
    {
        for (int i = 0 ; i < w ; ++i)
        {
            for (int j = 0 ; j < h ; ++j)
            {
                int v = pgm_read_dword(spr + i + j * w);
                if (v)
                    setPixColor(x + i, y + j, CRGB(v));
            }
        }
    }

private:
    int phase;
};