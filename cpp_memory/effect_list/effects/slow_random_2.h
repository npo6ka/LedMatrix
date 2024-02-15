#pragma once

#include "../effect.h"

class SlowRandom2 : public Effect
{
    Property<uint8_t> step;
    Property<uint64_t> val;

public:
    SlowRandom2() {}

    void on_init() {
        printf("Slow2 Init\n");
    }

    void on_update() {
        printf("Slow2 update\n");
    }
};
