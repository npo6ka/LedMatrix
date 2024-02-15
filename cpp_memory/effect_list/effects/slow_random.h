#pragma once

#include "../effect.h"

class SlowRandom : public Effect
{
    Property<int> step;

public:
    SlowRandom() {}

    void on_init() {
        printf("Slow1 Init\n");
    }

    void on_update() {
        printf("Slow1 update\n");
    }
};
