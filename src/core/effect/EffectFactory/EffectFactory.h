#pragma once

#include "effects/effect.h"
#include "EffectId.h"
#include "core/input/InputCapability.h"

#include <memory>

class EffectFactory {
public:
    static std::unique_ptr<Effect> createEffect(uint32_t effect_id);
    static const char* getEffectName(uint32_t effect_id);
    static InputCapability getRequiredCapabilities(uint32_t effect_id);
    static uint32_t getEffectCount();
};
