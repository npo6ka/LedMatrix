#pragma once

#include "effect_list/effect.h"

#define NY_SPRITE_W 10
#define NY_SPRITE_H 10

#define R 0xff0000
#define DR 0x400000
#define G 0x00ff00
#define DG 0x004000
#define B 0x0000ff
#define DB 0x000040
#define Y 0xffff00
#define C 0x00ffff
#define W 0xffffff
#define BR 0x402010
#define LB 0xc08040
#define O 0xff8000
#define P 0xffc0c0

static const uint32_t sprite1[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0, 0, 0, 0, W, 0, DR, DR, 0, 0,
    0, 0, 0, W, DB, W, DR, DR, 0, 0,
    0, 0, W, DB, DB, DB, W, DR, 0, 0,
    0, W, DB, DB, DB, DB, DB, W, 0, 0,
    W, DB, DB, DB, DB, DB, DB, DB, W, 0,
    0, BR, BR, BR, BR, BR, BR, BR, 0, 0,
    0, BR, Y, Y,   BR, Y, Y,   BR, 0, 0,
    0, BR, Y, Y,   BR, Y, Y,   BR, 0, 0,
    0, BR, BR, BR, BR, Y, Y,   BR, 0, 0,
    W, W,  W,  W,  W,  Y, Y,   W, W, W,
};

static const uint32_t sprite2[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0, 0,  0,  0,  W,  0,  0,  0,  0,  0,
    0, 0,  0,  W,  DB, W,  0,  0,  0,  0,
    0, 0,  W,  DB, DB, DB, W,  0,  0,  0,
    0, W,  DB, DB, DB, DB, DB, W,  0,  0,
    0, 0,  BR, BR, BR, BR, BR, 0,  0,  0,
    0, 0,  BR, C,  C,  C,  BR, 0,  0,  0,
    0, 0,  BR, C,  C,  C,  BR, 0,  0,  0,
    0, 0,  BR, BR, BR, BR, BR, 0,  0,  0,
    0, W,  BR, BR, BR, BR, BR, W,  W,  0,
    W, W,  W,  W,  W,  W,  W,  W,  W,  W,
};

static const uint32_t sprite3[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  G,  0,  0,  0,  0,  0,
    0,  0,  0,  G,  G,  G,  0,  0,  0,  0,
    0,  0,  0,  0,  G,  0,  0,  0,  0,  0,
    0,  0,  0,  G,  G,  G,  0,  0,  0,  0,
    0,  0,  G,  G,  G,  G,  G,  0,  0,  0,
    0,  0,  0,  0,  G,  0,  0,  0,  0,  0,
    0,  0,  G,  G,  G,  G,  G,  0,  0,  0,
    0,  G,  G,  G,  G,  G,  G,  G,  0,  0,
    0,  0,  0,  0,  G,  0,  0,  0,  0,  0,
};

static const uint32_t sprite4[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  G,  0,  0,  0,  0,  0,
    0,  0,  0,  G,  G,  G,  0,  0,  0,  0,
    0,  0,  0,  0,  G,  0,  0,  0,  0,  0,
    0,  0,  0,  G,  G,  G,  0,  0,  0,  0,
    0,  0,  G,  G,  G,  G,  G,  0,  0,  0,
    0,  0,  0,  0,  G,  0,  0,  0,  0,  0,
    0,  0,  G,  G,  G,  G,  G,  0,  0,  0,
    0,  G,  G,  G,  G,  G,  G,  G,  0,  0,
    0,  0,  0,  0,  G,  0,  0,  0,  0,  0,
};

static const uint32_t sprite5[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  W,  0,  0,  0,  0,  0,
    0,  0,  0,  W,  G,  W,  0,  0,  0,  0,
    0,  0,  W,  G,  G,  G,  W,  0,  0,  0,
    0,  W,  G,  G,  G,  G,  G,  W,  0,  0,
    0,  0,  W,  G,  G,  G,  W,  0,  0,  0,
    0,  W,  G,  G,  G,  G,  G,  W,  0,  0,
    W,  G,  G,  G,  G,  G,  G,  G,  W,  0,
    0,  W,  W,  G,  G,  G,  W,  W,  0,  0,
    0,  0,  0,  G,  G,  G,  0,  0,  0,  0,
};

static const uint32_t sprite6[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  Y,  0,  0,  0,  0,  0,
    0,  0,  0,  G,  G,  G,  0,  0,  0,  0,
    0,  0,  R,  O,  G,  O,  Y,  0,  0,  0,
    0,  0,  0,  G,  Y,  G,  0,  0,  0,  0,
    0,  0,  G,  G,  G,  G,  G,  0,  0,  0,
    0,  0,  Y,  G,  G,  G,  Y,  0,  0,  0,
    0,  R,  G,  O,  G,  O,  G,  R,  0,  0,
    O,  G,  G,  G,  R,  G,  G,  G,  O,  0,
    0,  0,  0,  G,  G,  G,  0,  0,  0,  0,
};

static const uint32_t sprite7[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  C,  0,  0,  0,  0,  0,
    0,  0,  C,  0,  C,  0,  C,  0,  0,  0,
    0,  C,  0,  C,  C,  C,  0,  C,  0,  0,
    0,  0,  C,  0,  C,  0,  C,  0,  0,  0,
    C,  C,  C,  C,  C,  C,  C,  C,  C,  0,
    0,  0,  C,  0,  C,  0,  C,  0,  0,  0,
    0,  C,  0,  C,  C,  C,  0,  C,  0,  0,
    0,  0,  C,  0,  C,  0,  C,  0,  0,  0,
    0,  0,  0,  0,  C,  0,  0,  0,  0,  0,
};

static const uint32_t sprite8[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  R,  0,  0,  0,  0,  0,  R,  0,  0,
    R,  0,  R,  0,  0,  0,  R,  0,  R,  0,
    R,  0,  0,  R,  0,  R,  0,  0,  R,  0,
    0,  R,  R,  R,  R,  R,  R,  R,  0,  0,
   DG, DG, DG, DG,  R, DG, DG, DG, DG,  0,
   DG, DG, DG, DG,  R, DG, DG, DG, DG,  0,
    R,  R,  R,  R,  R,  R,  R,  R,  R,  0,
   DG, DG, DG, DG,  R, DG, DG, DG, DG,  0,
   DG, DG, DG, DG,  R, DG, DG, DG, DG,  0,
};

static const uint32_t sprite9[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, BR,  0,  0,  0,  0,  0,  0,
    0,  0, BR, BR,  0,  0,  0,  0,  0,  0,
    0, LB, LB,  0,  0,  0,  0,  0,  0,  0,
   BR, LB, LB,  0,  0,  0,  0,  0,  0,  0,
    0,  0, LB, LB, LB, LB, LB, LB, LB,  0,
    0,  0, LB, LB, LB, LB, LB, LB,  0,  0,
    0, LB, LB, LB, LB, LB, LB, LB,  0,  0,
    0, BR,  0, LB,  0, LB,  0, LB,  0,  0,
    0,  0,  0, BR,  0, BR,  0, BR,  0,  0,
};

static const uint32_t sprite10[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  R,  R,  R,  0,  0,
    0,  0,  0,  0,  R,  R,  0,  R,  0,  0,
    0,  0,  0,  R,  R,  R,  0,  W,  W,  0,
    0,  0,  0,  R,  R,  R,  R,  W,  W,  0,
    0,  0,  R,  R,  R,  R,  R,  0,  0,  0,
    0,  0,  R,  R,  R,  R,  R,  R,  0,  0,
    0,  R,  R,  R,  R,  R,  R,  R,  0,  0,
    0,  R,  R,  R,  R,  R,  R,  R,  0,  0,
    W,  W,  W,  W,  W,  W,  W,  W,  W,  0,
    W,  W,  W,  W,  W,  W,  W,  W,  W,  0,
};

static const uint32_t sprite11[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0, BR, BR, BR,  0,  0,  0,  0,
    0, BR, BR, BR, BR, BR, BR, BR,  0,  0,
    0,  0,  W,  W,  W,  W,  W,  0,  0,  0,
    0,  W,  W,  0,  W,  0,  W,  W,  0,  0,
    0,  0,  W,  W,  W,  W,  W,  0,  0,  0,
    0,  0,  R,  R,  R,  R,  R,  0,  0,  0,
    0,  W,  W,  W,  R,  W,  W,  W,  0,  0,
    W,  W,  W,  W,  W,  R,  W,  W,  W,  0,
    0,  W,  W,  W,  0,  R,  W,  W,  0,  0,
    0,  0,  W,  W,  W,  W,  W,  0,  0,  0,
};

static const uint32_t sprite12[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  R,  R,  R,  R,  R,  R,  R,  R,  0,
    0,  W,  W,  W,  W,  W,  W,  0,  R,  0,
    0,  W,  R,  W,  R,  W,  R,  0,  R,  0,
    0,  R,  R,  R,  W,  R,  R,  0,  R,  0,
    0,  W,  R,  W,  R,  W,  R,  0,  R,  0,
    0,  W,  W,  W,  W,  W,  W,  0,  R,  0,
    0,  R,  R,  R,  R,  R,  R,  R,  R,  0,
    0,  R,  R,  R,  R,  R,  R,  0,  0,  0,
    0,  R,  R,  R,  R,  R,  R,  0,  0,  0,
};

static const uint32_t sprite13[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0, BR, BR,  0,  0,  0,  0,
    0,  0,  0,  0,  G,  G,  0,  0,  0,  0,
    0,  0,  0,  0,  G,  G,  0,  0,  0,  0,
    0,  0,  0,  0,  G,  G,  0,  0,  0,  0,
    0,  0,  0,  G,  G,  G,  G,  0,  0,  0,
    0,  0,  G,  G,  G,  G,  G,  G,  0,  0,
    0,  0,  W,  W,  W,  W,  W,  W,  0,  0,
    0,  0,  R,  R,  R,  R,  R,  R,  0,  0,
    0,  0,  G,  G,  G,  G,  G,  G,  0,  0,
    0,  0,  G,  G,  G,  G,  G,  G,  0,  0,
};

static const uint32_t sprite14[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  R,  R,  R,  0,  0,
    0,  0,  0,  0,  R,  R,  R,  0,  R,  0,
    0,  0,  0,  0,  P,  P,  P,  0,  W,  W,
    0,  0,  0,  0,  0,  P,  P,  0,  W,  W,
    0,  0,  0,  0,  0,  R, DR,  0,  0,  0,
    0,  0,  P,  R,  R, DR, DR,  0,  0,  R,
    0,  0,  0,  0, DR, DR, DR,  0,  R,  R,
   BR,  0,  0, DR, DR, DR, DR,  R,  R,  R,
   BR,  0,  R,  R,  R,  R,  R,  R,  R,  R,
    0, BR, BR, BR, BR, BR, BR, BR, BR, BR,
};

static const uint32_t sprite15[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  R,  0,  R,  0,  0,  0,  0,
    0,  0,  G,  R,  R,  R,  G,  0,  0,  0,
    0,  R,  G,  R,  0,  R,  G,  R,  0,  0,
    G,  R,  R,  0,  0,  0,  R,  R,  G,  0,
    G,  G,  0,  0,  0,  0,  0,  G,  G,  0,
    R,  R,  0,  0,  0,  0,  0,  R,  R,  0,
    R,  G,  G,  0,  0,  0,  G,  G,  R,  0,
    0,  G,  R,  0,  0,  R,  R,  G,  0,  0,
    0,  0,  R,  G,  R,  G,  R,  0,  0,  0,
    0,  0,  0,  G,  R,  G,  0,  0,  0,  0,
};

static const uint32_t sprite16[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  Y,  0,  0,  0,  0,  0,  0,
    0,  0,  Y,  Y,  0,  0,  0,  0,  0,  0,
    0,  0,  Y,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  W,  W,  0,  0,  0,  0,  0,  0,
    0,  0,  W,  W,  0,  0,  0,  0,  0,  0,
    0,  0,  W,  W,  0,  0,  0,  0,  0, LB,
    0,  0,  W,  W,  0,  0,  0,  0,  0, LB,
    0, LB,  W,  W, LB,  0,  0,  0, LB,  0,
    0, LB, LB, LB, LB, LB, LB, LB,  0,  0,
    0,  0, LB, LB, LB, LB,  0,  0,  0,  0,
};

static const uint32_t sprite17[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  G,  G,  R,  G,  G,  0,  0,  0,
    0,  0,  G,  R,  R,  R,  G,  0,  0,  0,
    0,  0,  0,  W,  W,  W,  0,  0,  0,  0,
    0,  0,  0,  W,  W,  W,  0,  0,  0,  0,
    0,  0,  W,  W,  W,  W,  W,  0,  0,  0,
    0,  0, BR,  W, BR,  W, BR,  0,  0,  0,
    0, BR, BR,  W, BR,  W, BR, BR,  0,  0,
    0, BR, BR, BR, BR,  W, BR, BR,  0,  0,
   BR, BR, BR, BR, BR, BR, BR, BR, BR,  0,
   BR, BR, BR, BR, BR, BR, BR, BR, BR,  0,
};

static const uint32_t sprite18[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  0,  0,  0,  W,  W,  R,  0,  0,
    0,  0,  0,  R,  W,  W,  R,  R,  W,  0,
    0,  0,  0,  R,  R,  0,  0,  W,  W,  0,
    0,  0,  0,  0,  0,  0,  R,  W,  W,  0,
    0,  0,  0,  0,  0,  W,  R,  R,  0,  0,
    0,  0,  0,  0,  R,  W,  W,  0,  0,  0,
    0,  0,  0,  W,  R,  R,  0,  0,  0,  0,
    0,  0,  R,  W,  W,  0,  0,  0,  0,  0,
    0,  W,  R,  R,  0,  0,  0,  0,  0,  0,
    0,  W,  W,  0,  0,  0,  0,  0,  0,  0,
};

static const uint32_t sprite19[NY_SPRITE_W*NY_SPRITE_H] PROGMEM = {
    0,  0,  W,  W,  W,  W,  W,  W,  W,  0,
    0,  0,  W,  W,  W,  W,  W,  W,  W,  0,
    0,  0,  0,  R,  R,  R,  R,  R,  0,  0,
    0,  0,  0,  R,  R,  G,  R,  R,  0,  0,
    0,  0,  0,  R,  G,  G,  G,  R,  0,  0,
    0,  0,  0,  R,  R,  G,  R,  R,  0,  0,
    0,  0,  R,  R,  R,  R,  W,  W,  0,  0,
    0,  R,  R,  R,  R,  R,  W,  W,  0,  0,
    W,  W,  R,  R,  R,  R,  W,  0,  0,  0,
    W,  W,  R,  R,  R,  R,  0,  0,  0,  0,
};

#undef R
#undef DR
#undef G
#undef DG
#undef B
#undef DB
#undef Y
#undef C
#undef W
#undef BR
#undef LB
#undef O
#undef P

#define NY_COUNT (LEDS_WIDTH / NY_SPRITE_W + 2)

static const uint32_t * const sprites[] =
{
    sprite1, sprite2, sprite3, sprite4, sprite5, sprite6, sprite7,
    sprite8, sprite9, sprite10, sprite11, sprite12, sprite13,
    sprite14, sprite15, sprite16, sprite17, sprite18, sprite19,
};

#define NY_TYPES (sizeof(sprites) / sizeof(sprites[0]))

class NY2020 : public Effect
{
public:
    NY2020() {}

    void on_init()
    {
        phase = 0;
        for (int i = 0 ; i < NY_COUNT ; ++i) {
            items[i] = random8(NY_TYPES);
        }

        set_fps(10);
    }

    void on_update()
    {
        FastLED.clear();
        for (int i = 0 ; i < NY_COUNT ; ++i) {
            int x = i * (NY_SPRITE_W + 1) - phase;
            int y = (LEDS_HEIGHT - NY_SPRITE_H) / 2;
            LedMatrix.drawSprite<NY_SPRITE_W, NY_SPRITE_H>(x, y, sprites[items[i]]);
        }
        phase = (phase + 1) % (NY_SPRITE_W + 1);
        if (phase == 0)
        {
            for (int i = 1 ; i < NY_COUNT ; ++i) {
                items[i - 1] = items[i];
            }
            items[NY_COUNT - 1] = random8(NY_TYPES);
        }
    }

private:
    int phase;
    int items[NY_COUNT];
};
