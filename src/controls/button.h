#pragma once
#include "GyverButton.h"

class Button {
    GButton touch;
public:
    Button(int8_t pin, bool type, bool dir);
    void onTick();
};