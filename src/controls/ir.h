#pragma once

#include "control.h"

class IR: public IControl {
    IR();
    IR(const IR& );
    IR& operator=(IR& );
public:
    static IR *instance();
    ControlState tick() final;
};