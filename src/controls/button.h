#pragma once

#include "icontrol.h"
#include "GyverButton.h"

class Button: public IControl {
    GButton touch;
public:
    Button(int8_t pin, bool type, bool dir);
    ControlState tick() final;
};