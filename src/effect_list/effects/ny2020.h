#pragma once

#include "effect_list/effect.h"

#define NY_SPRITE_W 10
#define NY_SPRITE_H 10

#define NY_COUNT (LEDS_WIDTH / NY_SPRITE_W + 2)

class NY2020 : public Effect
{
public:
    NY2020() {}

    void on_init();
    void on_update();

private:
    int phase;
    int items[NY_COUNT];
};
