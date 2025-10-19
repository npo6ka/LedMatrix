#pragma once

#include "effect_list/effect.h"

#include <stdint.h>

class EffectFactory {
    public:
        static Effect* createEffect(uint32_t effect_id);
        static const char* getEffectName(uint32_t effect_id);
        static uint32_t getEffectCount();
};


