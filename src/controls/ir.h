#pragma once

#include "control.h"

class IR: public IControl {
public:
    IR();
    ControlState tick() final;
};