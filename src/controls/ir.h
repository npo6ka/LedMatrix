#pragma once
#include "stdint.h"

class IR {
    IR(const IR& val) = delete;
    IR& operator=(IR& val) = delete;
public:
    IR(uint8_t pin);
    void onTick();
    bool isIdle();
};