#pragma once
#include "stdint.h"

class IR {
    IR(const IR& val) = delete;
    IR& operator=(const IR& val) = delete;
public:
    IR() {};
    void onInit(int pin);
    void onTick();
    bool isIdle();
};