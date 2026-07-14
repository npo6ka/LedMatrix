#pragma once

#include "effects/effect.h"

class Mouse : public Effect
{
public:
    Mouse();
    void on_init();
    void on_update();
private:
    int x;
};
