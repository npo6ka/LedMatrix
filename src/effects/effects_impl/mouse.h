#pragma once

#include "effects/effect.h"

class Mouse : public Effect
{
public:
    Mouse(const std::string& variableStoragePath) :
        Effect(variableStoragePath) {}
    void on_init();
    void on_update();
private:
    int x;
};
